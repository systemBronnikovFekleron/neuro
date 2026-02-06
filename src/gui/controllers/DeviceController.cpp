#include "controllers/DeviceController.h"
#include "CapsuleManager.h"
#include "SessionManager.h"
#include "MetricsCollector.h"
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <iostream>

DeviceController::DeviceController(QObject* parent)
    : QObject(parent)
{
    // НОВОЕ: Инициализация таймера для асинхронных операций
    m_asyncTimer = new QTimer(this);
    m_asyncTimer->setInterval(50);  // 50ms между update() вызовами
    connect(m_asyncTimer, &QTimer::timeout, this, &DeviceController::onAsyncUpdate);
}

DeviceController::~DeviceController() = default;

void DeviceController::setCapsuleManager(Bronnikov::CapsuleManager* manager)
{
    m_capsuleManager = manager;
    setupCallbacks();
}

void DeviceController::discoverDevices()
{
    if (!m_capsuleManager) {
        emit errorOccurred("CapsuleManager not initialized");
        return;
    }

    // Защита от множественных вызовов
    if (m_currentOperation != AsyncOperation::None) {
        qDebug() << "⚠ Уже выполняется операция, игнорируем поиск";
        return;
    }

    qDebug() << "🔍 Поиск устройств Neiry...";
    m_connectionStatus = "Searching...";
    emit connectionStatusChanged();
    emit searchProgress("Поиск устройств...");

    // Режим "только поиск" - без автоподключения
    m_targetDeviceId.clear();
    m_wantAutoConnect = false;

    // Проверяем подключение к backend
    if (!m_capsuleManager->isConnected()) {
        qDebug() << "  Подключение к Capsule backend...";
        if (!m_capsuleManager->connect("inproc://capsule")) {
            qDebug() << "❌ Не удалось подключиться к Capsule";
            emit errorOccurred("Failed to connect to Capsule backend");
            m_connectionStatus = "Disconnected";
            emit connectionStatusChanged();
            return;
        }
        emit connectionProgress("Подключение к Capsule backend...");
        startAsyncOperation(AsyncOperation::ConnectingToBackend);
    } else {
        startAsyncOperation(AsyncOperation::DiscoveringDevices);
    }
}

void DeviceController::connectToDevice(const QString& deviceId)
{
    if (!m_capsuleManager) {
        emit connectionFailed("CapsuleManager not initialized");
        return;
    }

    // Защита от множественных вызовов
    if (m_currentOperation != AsyncOperation::None) {
        qDebug() << "⚠ Уже выполняется операция, игнорируем";
        return;
    }

    qDebug() << "🔌 Подключение к устройству..." << (deviceId.isEmpty() ? "(авто)" : deviceId);
    m_connectionStatus = "Connecting...";
    emit connectionStatusChanged();

    m_targetDeviceId = deviceId;
    m_wantAutoConnect = true;  // Хотим подключиться после поиска

    // Проверяем, подключены ли мы к Capsule backend
    if (!m_capsuleManager->isConnected()) {
        qDebug() << "  Подключение к Capsule backend...";
        if (!m_capsuleManager->connect("inproc://capsule")) {
            qDebug() << "❌ Не удалось подключиться к Capsule";
            emit connectionFailed("Failed to connect to Capsule backend");
            m_connectionStatus = "Disconnected";
            emit connectionStatusChanged();
            return;
        }

        emit connectionProgress("Подключение к Capsule backend...");
        startAsyncOperation(AsyncOperation::ConnectingToBackend);
    } else if (!deviceId.isEmpty() && m_capsuleManager->hasLocator()) {
        // Если устройство уже найдено (есть locator) и указан конкретный ID - подключаемся напрямую
        qDebug() << "  Прямое подключение к известному устройству:" << deviceId;
        emit connectionProgress(QString("Подключение к %1...").arg(deviceId));
        startAsyncOperation(AsyncOperation::ConnectingToDevice);
    } else {
        // Нужен поиск устройств
        emit connectionProgress("Поиск устройств...");
        startAsyncOperation(AsyncOperation::DiscoveringDevices);
    }
}

void DeviceController::disconnectFromDevice()
{
    if (!m_capsuleManager) {
        return;
    }

    qDebug() << "🔌 Отключение от устройства...";

    // Останавливаем сессию если активна
    if (m_isSessionActive) {
        stopSession();
    }

    // Отключаемся от устройства
    m_capsuleManager->disconnectDevice();

    // Даем время на обработку отключения
    for (int i = 0; i < 10; ++i) {
        m_capsuleManager->update();
        QThread::msleep(50);
    }

    m_isConnected = false;
    m_deviceName.clear();
    m_batteryLevel = 0;
    m_signalQualityChannel1 = 0;
    m_signalQualityChannel2 = 0;
    updateConnectionStatus();

    qDebug() << "✓ Устройство отключено";

    emit isConnectedChanged();
    emit deviceNameChanged();
    emit batteryLevelChanged();
    emit signalQualityChanged();
    emit connectionStatusChanged();
}

void DeviceController::startSession()
{
    if (!m_isConnected) {
        emit errorOccurred("Device not connected");
        return;
    }

    if (!m_capsuleManager || !m_capsuleManager->isDeviceConnected()) {
        emit errorOccurred("Device not connected");
        return;
    }

    qDebug() << "🎬 Запуск EEG сессии...";
    m_connectionStatus = "Starting session...";
    emit connectionStatusChanged();

    // Создаем SessionManager
    if (!m_sessionManager) {
        qDebug() << "  Создание SessionManager...";

        // Создаем SessionManager с помощью CapsuleManager
        auto client = m_capsuleManager->getClient();
        auto device = m_capsuleManager->getDevice();

        if (!client || !device) {
            qDebug() << "❌ Не удалось получить client/device";
            emit errorOccurred("Failed to get client/device from CapsuleManager");
            m_connectionStatus = "Connected";
            emit connectionStatusChanged();
            return;
        }

        m_sessionManager = std::make_unique<Bronnikov::SessionManager>(client, device);
        qDebug() << "✓ SessionManager создан";
    }

    // Запускаем сессию
    if (!m_sessionManager->startSession("gui_session")) {
        qDebug() << "❌ Не удалось запустить сессию";
        emit errorOccurred("Failed to start session");
        m_connectionStatus = "Connected";
        emit connectionStatusChanged();
        return;
    }

    // Ждем запуска сессии (с обработкой UI событий)
    for (int i = 0; i < 50 && !m_sessionManager->isActive(); ++i) {
        m_capsuleManager->update();
        QCoreApplication::processEvents();
        QThread::msleep(50);
    }

    if (!m_sessionManager->isActive()) {
        qDebug() << "❌ Timeout при запуске сессии";
        emit errorOccurred("Timeout starting session");
        m_sessionManager.reset();
        m_connectionStatus = "Connected";
        emit connectionStatusChanged();
        return;
    }

    qDebug() << "✓ Сессия запущена";

    // Создаем MetricsCollector
    qDebug() << "  Создание MetricsCollector...";
    auto session = m_sessionManager->getSession();

    if (!session) {
        qDebug() << "❌ Не удалось получить session";
        emit errorOccurred("Failed to get session");
        m_sessionManager->stopSession();
        m_sessionManager.reset();
        m_connectionStatus = "Connected";
        emit connectionStatusChanged();
        return;
    }

    m_metricsCollector = std::make_unique<Bronnikov::MetricsCollector>(session);

    if (!m_metricsCollector->initialize()) {
        qDebug() << "❌ Не удалось инициализировать MetricsCollector";
        emit errorOccurred("Failed to initialize metrics collector");
        m_metricsCollector.reset();
        m_sessionManager->stopSession();
        m_sessionManager.reset();
        m_connectionStatus = "Connected";
        emit connectionStatusChanged();
        return;
    }

    qDebug() << "✓ MetricsCollector инициализирован";

    // Запускаем режимы устройства для сбора данных
    qDebug() << "  Запуск режимов устройства (Signal + PPG)...";
    m_capsuleManager->switchDeviceMode(clC_DM_Signal);  // EEG данные

    // Даем время на переключение режима (с обработкой UI событий)
    for (int i = 0; i < 3; ++i) {
        m_capsuleManager->update();
        QCoreApplication::processEvents();
        QThread::msleep(30);
    }

    m_capsuleManager->switchDeviceMode(clC_DM_StartPPG);  // Кардио данные (PPG)

    // Даем время на переключение режима (с обработкой UI событий)
    for (int i = 0; i < 3; ++i) {
        m_capsuleManager->update();
        QCoreApplication::processEvents();
        QThread::msleep(30);
    }

    qDebug() << "✓ Режимы устройства запущены";

    m_isSessionActive = true;
    updateConnectionStatus();

    // КРИТИЧНО: Запускаем таймер для постоянного вызова update() во время сессии
    // Без этого callbacks от CapsuleAPI не будут вызываться!
    if (!m_asyncTimer->isActive()) {
        qDebug() << "🔄 Запуск update loop для обработки callbacks...";
        m_asyncTimer->start();
    }

    emit isSessionActiveChanged();
    emit sessionStarted();
}

void DeviceController::stopSession()
{
    if (!m_isSessionActive || !m_sessionManager) {
        return;
    }

    qDebug() << "⏹️ Остановка EEG сессии...";

    // Останавливаем сессию
    m_sessionManager->stopSession();

    // Ждем остановки (с обработкой UI событий)
    for (int i = 0; i < 20 && m_sessionManager->isActive(); ++i) {
        m_capsuleManager->update();
        QCoreApplication::processEvents();
        QThread::msleep(50);
    }

    // Уничтожаем MetricsCollector и SessionManager
    m_metricsCollector.reset();
    m_sessionManager.reset();

    qDebug() << "✓ Сессия остановлена";

    m_isSessionActive = false;
    updateConnectionStatus();

    emit isSessionActiveChanged();
    emit sessionStopped();
}

void DeviceController::startCalibration()
{
    if (!m_isSessionActive) {
        emit calibrationFailed("Session not active");
        return;
    }

    if (!m_metricsCollector) {
        emit calibrationFailed("MetricsCollector not initialized");
        return;
    }

    qDebug() << "🎯 Запуск калибровки NFB (90 секунд)...";
    m_isCalibrating = true;
    m_calibrationProgress = 0;
    m_operationStep = 0;  // КРИТИЧНО: Сброс счетчика для прогресса калибровки

    emit isCalibratingChanged();
    emit calibrationProgressChanged();

    // Устанавливаем callback для завершения калибровки
    m_metricsCollector->setOnCalibrationCompleteCallback([this](bool success, double iaf) {
        qDebug() << "Калибровка завершена. Успех:" << success << "IAF:" << iaf;

        if (success) {
            m_iaf = iaf;
            m_iapf = iaf;  // Упрощенно - используем IAF как IAPF
            m_isCalibrating = false;
            m_calibrationProgress = 100;

            qDebug() << "✓ Калибровка успешна! IAF:" << m_iaf << "Hz";

            emit iafChanged();
            emit iapfChanged();
            emit isCalibratingChanged();
            emit calibrationProgressChanged();
            emit calibrationCompleted(m_iaf, m_iapf);
        } else {
            m_isCalibrating = false;
            m_calibrationProgress = 0;

            qDebug() << "❌ Калибровка не удалась";

            emit isCalibratingChanged();
            emit calibrationProgressChanged();
            emit calibrationFailed("Calibration failed");
        }
    });

    // Запускаем калибровку с повторными попытками (калибратор может быть не готов)
    bool started = false;
    for (int attempt = 0; attempt < 10 && !started; ++attempt) {
        if (attempt > 0) {
            qDebug() << "  Попытка" << (attempt + 1) << "...";
            // Ждём и обновляем для получения события OnReadyToCalibrate
            for (int i = 0; i < 20; ++i) {
                if (m_capsuleManager) m_capsuleManager->update();
                QThread::msleep(100);
            }
        }
        started = m_metricsCollector->startCalibration(90);
    }

    if (!started) {
        qDebug() << "❌ Не удалось запустить калибровку после 10 попыток";
        m_isCalibrating = false;
        emit isCalibratingChanged();
        emit calibrationFailed("Calibrator not ready. Try again in a few seconds.");
        return;
    }

    qDebug() << "✓ Калибровка запущена. Длительность: 90 секунд";
    qDebug() << "  Закройте глаза и расслабьтесь...";
}

void DeviceController::cancelCalibration()
{
    // TODO: Добавить метод stopCalibration() в MetricsCollector если нужно
    m_isCalibrating = false;
    m_calibrationProgress = 0;

    emit isCalibratingChanged();
    emit calibrationProgressChanged();

    qDebug() << "⏹️ Калибровка отменена";
}

QVariantMap DeviceController::getDeviceInfo() const
{
    QVariantMap info;
    info["name"] = m_deviceName;
    info["battery"] = m_batteryLevel;
    info["connected"] = m_isConnected;
    info["sessionActive"] = m_isSessionActive;
    return info;
}

QVariantMap DeviceController::getCurrentMetrics() const
{
    QVariantMap metrics;

    if (!m_metricsCollector) {
        // Возвращаем нулевые метрики если коллектор не создан
        metrics["alpha"] = 0.0;
        metrics["beta"] = 0.0;
        metrics["theta"] = 0.0;
        metrics["concentration"] = 0.0;
        metrics["relaxation"] = 0.0;
        metrics["fatigue"] = 0.0;
        metrics["heartRate"] = 0.0;
        return metrics;
    }

    // Получаем текущие метрики из коллектора
    auto snapshot = m_metricsCollector->getCurrentMetrics();

    metrics["alpha"] = snapshot.alpha_power;
    metrics["beta"] = snapshot.beta_power;
    metrics["theta"] = snapshot.theta_power;
    metrics["concentration"] = snapshot.concentration;
    metrics["relaxation"] = snapshot.relaxation;
    metrics["fatigue"] = snapshot.fatigue;
    metrics["focus"] = snapshot.focus;
    metrics["stress"] = snapshot.stress;
    metrics["heartRate"] = snapshot.heart_rate;

    // DEBUG: Логируем что возвращаем
    static int metrics_call_count = 0;
    if (++metrics_call_count % 50 == 0) {
        qDebug() << "[DEBUG] getCurrentMetrics called #" << metrics_call_count
                 << "alpha=" << snapshot.alpha_power
                 << "beta=" << snapshot.beta_power
                 << "theta=" << snapshot.theta_power;
    }

    return metrics;
}

void DeviceController::onDeviceConnected()
{
    m_isConnected = true;
    updateConnectionStatus();
    emit isConnectedChanged();
    emit connectionStatusChanged();
}

void DeviceController::onDeviceDisconnected()
{
    m_isConnected = false;
    m_isSessionActive = false;
    updateConnectionStatus();
    emit isConnectedChanged();
    emit isSessionActiveChanged();
    emit connectionStatusChanged();
}

void DeviceController::onSessionStateChanged()
{
    // TODO: Query actual session state
}

void DeviceController::onBatteryLevelChanged(int level)
{
    if (m_batteryLevel != level) {
        m_batteryLevel = level;
        emit batteryLevelChanged();
    }
}

void DeviceController::onSignalQualityChanged(int ch1, int ch2)
{
    bool changed = false;

    if (m_signalQualityChannel1 != ch1) {
        m_signalQualityChannel1 = ch1;
        changed = true;
    }

    if (m_signalQualityChannel2 != ch2) {
        m_signalQualityChannel2 = ch2;
        changed = true;
    }

    if (changed) {
        emit signalQualityChanged();
    }
}

void DeviceController::onCalibrationProgressChanged(int progress)
{
    if (m_calibrationProgress != progress) {
        m_calibrationProgress = progress;
        emit calibrationProgressChanged();
    }
}

void DeviceController::onCalibrationCompleted(double iaf, double iapf)
{
    m_iaf = iaf;
    m_iapf = iapf;
    m_isCalibrating = false;
    m_calibrationProgress = 100;

    emit iafChanged();
    emit iapfChanged();
    emit isCalibratingChanged();
    emit calibrationProgressChanged();
    emit calibrationCompleted(iaf, iapf);
}

void DeviceController::onAsyncUpdate()
{
    if (!m_capsuleManager) {
        stopAsyncOperation();
        return;
    }

    // КРИТИЧНО: Вызываем update() ВСЕГДА для обработки callbacks от CapsuleAPI
    m_capsuleManager->update();

    // Увеличиваем счетчик шагов если есть активная операция ИЛИ калибровка
    if (m_currentOperation != AsyncOperation::None || m_isCalibrating) {
        m_operationStep++;
    }

    // НОВОЕ: Обновляем прогресс калибровки если активна
    if (m_isCalibrating && m_metricsCollector && m_metricsCollector->isCalibrating()) {
        // Примерный прогресс: 90 секунд = 1800 шагов по 50ms
        int estimated_progress = std::min(99, (m_operationStep * 100) / 1800);
        if (m_calibrationProgress != estimated_progress) {
            m_calibrationProgress = estimated_progress;
            emit calibrationProgressChanged();
        }
    }

    switch (m_currentOperation) {
        case AsyncOperation::ConnectingToBackend:
            if (m_capsuleManager->isConnected()) {
                qDebug() << "✓ Подключено к Capsule backend";
                emit connectionProgress("Подключено к Capsule backend");
                stopAsyncOperation();
                // Переходим к поиску устройств
                startAsyncOperation(AsyncOperation::DiscoveringDevices);
                emit connectionProgress("Поиск устройств...");
            } else if (m_operationStep > 200) {  // 10 секунд timeout
                qDebug() << "❌ Timeout при подключении к Capsule";
                emit connectionFailed("Timeout connecting to Capsule backend");
                stopAsyncOperation();
            }
            break;

        case AsyncOperation::DiscoveringDevices:
            if (m_operationStep == 1) {
                // Первый шаг - запускаем асинхронный поиск
                m_capsuleManager->discoverDevices();
                qDebug() << "  Поиск запущен, ожидание результатов (callback)...";
            } else if (m_operationStep > 100) {  // 5 секунд timeout
                qDebug() << "⚠ Timeout при поиске устройств";
                emit searchProgress("Timeout при поиске устройств");

                // ИСПРАВЛЕНО: Обновляем статус подключения после timeout
                m_connectionStatus = "Disconnected";
                emit connectionStatusChanged();

                stopAsyncOperation();

                // Если была команда подключения, сообщаем об ошибке
                if (m_wantAutoConnect) {
                    emit connectionFailed("Device discovery timeout");
                }
            }
            // ВАЖНО: Callback от CapsuleManager вызовет deviceDiscovered() и stopAsyncOperation()
            break;

        case AsyncOperation::ConnectingToDevice:
            if (m_operationStep == 1) {
                // Первый шаг - запускаем подключение
                if (!m_capsuleManager->connectToDevice(m_targetDeviceId.toStdString())) {
                    qDebug() << "❌ Не удалось запустить подключение";
                    emit connectionFailed("Failed to initiate device connection");
                    stopAsyncOperation();
                }
            } else if (m_capsuleManager->isDeviceConnected()) {
                qDebug() << "✓ Устройство подключено:" << m_targetDeviceId;
                m_isConnected = true;
                m_deviceName = m_targetDeviceId;
                updateConnectionStatus();
                emit isConnectedChanged();
                emit deviceNameChanged();
                emit connectionStatusChanged();

                // Переходим к ожиданию данных батареи
                startAsyncOperation(AsyncOperation::WaitingForDeviceData);
                emit connectionProgress("Получение данных устройства...");
            } else if (m_operationStep > 1000) {  // 50 секунд timeout
                qDebug() << "❌ Timeout при подключении к устройству";
                emit connectionFailed("Timeout connecting to device");
                stopAsyncOperation();
            }
            break;

        case AsyncOperation::WaitingForDeviceData:
            if (m_operationStep > 20) {  // Ждем 1 секунду для получения данных
                m_batteryLevel = m_capsuleManager->getBatteryLevel();
                m_signalQualityChannel1 = m_capsuleManager->getSignalQuality();
                m_signalQualityChannel2 = m_capsuleManager->getSignalQuality();

                qDebug() << "  Батарея:" << m_batteryLevel << "%";
                emit batteryLevelChanged();
                emit signalQualityChanged();

                // ИСПРАВЛЕНО: Не останавливаем таймер полностью, переходим в режим мониторинга
                m_currentOperation = AsyncOperation::None;
                m_operationStep = 0;
                // Таймер продолжает работать для периодических обновлений
                emit connectionProgress("Подключено");
            }
            break;

        case AsyncOperation::StartingSession:
            // TODO: Асинхронный запуск сессии (если нужно)
            break;

        case AsyncOperation::WaitingForSessionStart:
            // TODO: Ожидание запуска сессии
            break;

        default:
            // Мониторинг состояния устройства когда подключены но нет активной операции
            if (m_isConnected && m_capsuleManager) {
                // Периодически (каждые 2 секунды = 40 шагов) обновляем данные устройства
                static int monitor_counter = 0;
                if (++monitor_counter >= 40) {
                    monitor_counter = 0;

                    int newBattery = m_capsuleManager->getBatteryLevel();
                    if (newBattery != m_batteryLevel && newBattery > 0) {
                        m_batteryLevel = newBattery;
                        emit batteryLevelChanged();
                    }

                    int newQuality = m_capsuleManager->getSignalQuality();
                    if (newQuality != m_signalQualityChannel1) {
                        m_signalQualityChannel1 = newQuality;
                        m_signalQualityChannel2 = newQuality;
                        emit signalQualityChanged();
                    }
                }
            }
            break;
    }
}

void DeviceController::updateConnectionStatus()
{
    if (m_isConnected) {
        if (m_isSessionActive) {
            m_connectionStatus = "Session Active";
        } else {
            m_connectionStatus = "Connected";
        }
    } else {
        m_connectionStatus = "Disconnected";
    }
}

void DeviceController::startAsyncOperation(AsyncOperation operation)
{
    m_currentOperation = operation;
    m_operationStep = 0;
    m_asyncTimer->start();
}

void DeviceController::stopAsyncOperation()
{
    // ИСПРАВЛЕНО: Не останавливаем таймер если устройство подключено (нужен для мониторинга)
    if (!m_isConnected) {
        m_asyncTimer->stop();
    }
    m_currentOperation = AsyncOperation::None;
    m_operationStep = 0;
}

void DeviceController::setupCallbacks()
{
    if (!m_capsuleManager) {
        return;
    }

    // Callback при нахождении устройств
    m_capsuleManager->setOnDevicesFoundCallback([this](const std::vector<std::string>& device_ids) {
        qDebug() << "✓ [DeviceController] Обнаружено устройств:" << device_ids.size();

        if (device_ids.empty()) {
            qDebug() << "  Устройства не найдены";
            emit searchProgress("Устройства не найдены");

            if (m_wantAutoConnect) {
                emit connectionFailed("No devices found");
            }

            stopAsyncOperation();
            // ИСПРАВЛЕНО: Сбрасываем статус после поиска
            updateConnectionStatus();
            emit connectionStatusChanged();
            return;
        }

        for (const auto& device_id : device_ids) {
            // Эмитируем сигнал для каждого найденного устройства
            // TODO: Получить реальное имя устройства из CapsuleAPI
            QString qDeviceId = QString::fromStdString(device_id);
            QString qDeviceName = QString("Neiry Device (%1)").arg(qDeviceId.left(8));

            qDebug() << "  Найдено:" << qDeviceId;
            emit deviceDiscovered(qDeviceId, qDeviceName);
            emit searchProgress(QString("Найдено: %1").arg(qDeviceName));
        }

        // Если был запрос на подключение, подключаемся
        if (m_wantAutoConnect) {
            // Находим нужное устройство или берем первое
            bool found = false;
            for (const auto& device_id : device_ids) {
                QString qDeviceId = QString::fromStdString(device_id);
                if (m_targetDeviceId.isEmpty() || m_targetDeviceId == qDeviceId) {
                    m_targetDeviceId = qDeviceId;
                    found = true;
                    break;
                }
            }

            if (found) {
                qDebug() << "  Автоподключение к:" << m_targetDeviceId;
                stopAsyncOperation();
                startAsyncOperation(AsyncOperation::ConnectingToDevice);
                emit connectionProgress(QString("Подключение к %1...").arg(m_targetDeviceId));
            } else {
                qDebug() << "❌ Целевое устройство не найдено:" << m_targetDeviceId;
                emit connectionFailed("Target device not found");
                stopAsyncOperation();
                updateConnectionStatus();
                emit connectionStatusChanged();
            }
        } else {
            // Просто поиск, останавливаем операцию
            qDebug() << "  Поиск завершён (без автоподключения)";
            stopAsyncOperation();
            // ИСПРАВЛЕНО: Сбрасываем статус после поиска
            updateConnectionStatus();
            emit connectionStatusChanged();
        }
    });

    // Callback при ошибке
    m_capsuleManager->setOnErrorCallback([this](const std::string& error) {
        QString qError = QString::fromStdString(error);
        std::cerr << "[DeviceController] Ошибка: " << error << std::endl;
        emit errorOccurred(qError);

        // Останавливаем текущую операцию при ошибке
        stopAsyncOperation();
        // ИСПРАВЛЕНО: Сбрасываем статус после ошибки
        updateConnectionStatus();
        emit connectionStatusChanged();
    });

    // Callback при подключении к backend
    m_capsuleManager->setOnConnectedCallback([this](bool success) {
        if (success) {
            std::cout << "[DeviceController] Подключено к Capsule backend" << std::endl;
        } else {
            std::cerr << "[DeviceController] Не удалось подключиться к Capsule backend" << std::endl;
            emit connectionFailed("Не удалось подключиться к Capsule backend");
        }
    });

    // Callback при изменении уровня батареи
    m_capsuleManager->setOnBatteryUpdateCallback([this](int level) {
        if (m_batteryLevel != level) {
            m_batteryLevel = level;
            emit batteryLevelChanged();
        }
    });
}
