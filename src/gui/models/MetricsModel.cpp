#include "models/MetricsModel.h"
#include "controllers/DeviceController.h"
#include "MetricsCollector.h"
#include <cmath>      // для std::sin, std::max, std::min
#include <cstdlib>    // для rand()

MetricsModel::MetricsModel(QObject* parent)
    : QObject(parent)
    , m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, &MetricsModel::updateMetrics);
}

MetricsModel::~MetricsModel() = default;

QVariantList MetricsModel::alphaHistory() const
{
    QVariantList result;
    for (double value : m_alphaHistory) {
        result.append(value);
    }
    return result;
}

QVariantList MetricsModel::betaHistory() const
{
    QVariantList result;
    for (double value : m_betaHistory) {
        result.append(value);
    }
    return result;
}

QVariantList MetricsModel::thetaHistory() const
{
    QVariantList result;
    for (double value : m_thetaHistory) {
        result.append(value);
    }
    return result;
}

void MetricsModel::startMonitoring()
{
    if (!m_isActive) {
        m_isActive = true;
        m_updateTimer->start(100);  // Update every 100ms
        emit isActiveChanged();
    }
}

void MetricsModel::stopMonitoring()
{
    if (m_isActive) {
        m_isActive = false;
        m_updateTimer->stop();
        emit isActiveChanged();
    }
}

void MetricsModel::reset()
{
    m_alpha = 0.0;
    m_beta = 0.0;
    m_theta = 0.0;
    m_concentration = 0.0;
    m_relaxation = 0.0;
    m_fatigue = 0.0;
    m_focus = 0.0;
    m_stress = 0.0;
    m_chill = 0.0;
    m_heartRate = 0;
    m_stressIndex = 0.0;
    m_successRate = 0.0;

    m_alphaHistory.clear();
    m_betaHistory.clear();
    m_thetaHistory.clear();

    emit alphaChanged();
    emit betaChanged();
    emit thetaChanged();
    emit concentrationChanged();
    emit relaxationChanged();
    emit fatigueChanged();
    emit focusChanged();
    emit stressChanged();
    emit chillChanged();
    emit heartRateChanged();
    emit stressIndexChanged();
    emit successRateChanged();
    emit alphaHistoryChanged();
    emit betaHistoryChanged();
    emit thetaHistoryChanged();
}

void MetricsModel::setMetricsCollector(void* collector)
{
    m_metricsCollector = static_cast<Bronnikov::MetricsCollector*>(collector);
}

void MetricsModel::setDeviceController(QObject* controller)
{
    m_deviceController = qobject_cast<DeviceController*>(controller);

    // Автоматически включаем мониторинг если устройство подключено и сессия активна
    if (m_deviceController && m_deviceController->isSessionActive()) {
        m_demoMode = false;  // Отключаем демо-режим
        startMonitoring();
    }
}

void MetricsModel::updateMetrics()
{
    // DEBUG
    static int update_count = 0;
    if (++update_count % 50 == 0) {
        qDebug() << "[MetricsModel] updateMetrics called #" << update_count
                 << "demoMode=" << m_demoMode
                 << "hasDeviceController=" << (m_deviceController != nullptr)
                 << "isSessionActive=" << (m_deviceController ? m_deviceController->isSessionActive() : false);
    }

    // НОВОЕ: Проверяем демо-режим
    if (m_demoMode) {
        generateDemoMetrics();
        return;
    }

    // Приоритет 1: Данные из DeviceController (через Qt/QML)
    if (m_deviceController && m_deviceController->isSessionActive()) {
        QVariantMap metricsMap = m_deviceController->getCurrentMetrics();

        // Конвертируем QVariantMap в MetricsSnapshot
        Bronnikov::MetricsSnapshot snapshot;
        snapshot.alpha_power = metricsMap["alpha"].toDouble();
        snapshot.beta_power = metricsMap["beta"].toDouble();
        snapshot.theta_power = metricsMap["theta"].toDouble();
        snapshot.concentration = metricsMap["concentration"].toDouble();
        snapshot.relaxation = metricsMap["relaxation"].toDouble();
        snapshot.fatigue = metricsMap["fatigue"].toDouble();
        snapshot.focus = metricsMap["focus"].toDouble();
        snapshot.stress = metricsMap["stress"].toDouble();
        snapshot.heart_rate = metricsMap["heartRate"].toDouble();

        updateFromSnapshot(snapshot);
        return;
    }

    // Приоритет 2: Прямой доступ к MetricsCollector (legacy)
    if (m_metricsCollector) {
        auto snapshot = m_metricsCollector->getCurrentMetrics();
        updateFromSnapshot(snapshot);
        return;
    }

    // Если нет источника данных - ничего не делаем
}

void MetricsModel::updateFromSnapshot(const Bronnikov::MetricsSnapshot& snapshot)
{
    // DEBUG
    static int snapshot_count = 0;
    if (++snapshot_count % 50 == 0) {
        qDebug() << "[MetricsModel] updateFromSnapshot #" << snapshot_count
                 << "alpha=" << snapshot.alpha_power
                 << "beta=" << snapshot.beta_power
                 << "theta=" << snapshot.theta_power
                 << "m_alpha=" << m_alpha << " (old)";
    }

    // Update current values
    // ВАЖНО: Масштабируем значения 0.0-1.0 → 0-100 для отображения в UI
    double alphaScaled = snapshot.alpha_power * 100.0;
    if (m_alpha != alphaScaled) {
        m_alpha = alphaScaled;
        addToHistory(m_alphaHistory, m_alpha);
        emit alphaChanged();
        emit alphaHistoryChanged();

        // DEBUG
        if (snapshot_count % 50 == 0) {
            qDebug() << "  [MetricsModel] EMIT alphaChanged! new value=" << m_alpha;
        }
    }

    double betaScaled = snapshot.beta_power * 100.0;
    if (m_beta != betaScaled) {
        m_beta = betaScaled;
        addToHistory(m_betaHistory, m_beta);
        emit betaChanged();
        emit betaHistoryChanged();
    }

    double thetaScaled = snapshot.theta_power * 100.0;
    if (m_theta != thetaScaled) {
        m_theta = thetaScaled;
        addToHistory(m_thetaHistory, m_theta);
        emit thetaChanged();
        emit thetaHistoryChanged();
    }

    // ВАЖНО: Concentration/Relaxation/Fatigue приходят в диапазоне ~0-5
    // Масштабируем в 0-100 для отображения (×20)
    double concentrationScaled = snapshot.concentration * 20.0;
    if (m_concentration != concentrationScaled) {
        m_concentration = concentrationScaled;
        emit concentrationChanged();
    }

    double relaxationScaled = snapshot.relaxation * 20.0;
    if (m_relaxation != relaxationScaled) {
        m_relaxation = relaxationScaled;
        emit relaxationChanged();
    }

    double fatigueScaled = snapshot.fatigue * 20.0;
    if (m_fatigue != fatigueScaled) {
        m_fatigue = fatigueScaled;
        emit fatigueChanged();
    }

    if (m_focus != snapshot.focus) {
        m_focus = snapshot.focus;
        emit focusChanged();
    }

    if (m_stress != snapshot.stress) {
        m_stress = snapshot.stress;
        emit stressChanged();
    }

    if (m_chill != snapshot.chill) {
        m_chill = snapshot.chill;
        emit chillChanged();
    }

    if (m_heartRate != snapshot.heart_rate) {
        m_heartRate = snapshot.heart_rate;
        emit heartRateChanged();
    }

    if (m_stressIndex != snapshot.stress_index) {
        m_stressIndex = snapshot.stress_index;
        emit stressIndexChanged();
    }
}

void MetricsModel::addToHistory(std::deque<double>& history, double value)
{
    history.push_back(value);
    if (history.size() > MAX_HISTORY_SIZE) {
        history.pop_front();
    }
}

// НОВОЕ: Запуск демо-режима с fake данными
void MetricsModel::startDemoMode()
{
    if (m_isActive) {
        return;  // Уже активен
    }

    m_demoMode = true;
    m_demoTime = 0.0;
    m_isActive = true;

    // Запускаем таймер обновления
    m_updateTimer->start(100);  // 100ms обновления

    emit isActiveChanged();
}

// НОВОЕ: Генерация демо-данных
void MetricsModel::generateDemoMetrics()
{
    // Увеличиваем время
    m_demoTime += 0.1;  // 100ms = 0.1s

    // Генерируем синусоидальные волны с разными частотами и амплитудами
    // Alpha (8-12 Hz) - доминирующая при расслаблении
    double newAlpha = 60.0 + 20.0 * std::sin(m_demoTime * 0.5) + 10.0 * (rand() % 100) / 100.0;

    // Beta (13-30 Hz) - активность, концентрация
    double newBeta = 45.0 + 15.0 * std::sin(m_demoTime * 0.8) + 8.0 * (rand() % 100) / 100.0;

    // Theta (4-7 Hz) - медитация, глубокое расслабление
    double newTheta = 35.0 + 12.0 * std::sin(m_demoTime * 0.3) + 5.0 * (rand() % 100) / 100.0;

    // Обновляем значения
    if (m_alpha != newAlpha) {
        m_alpha = newAlpha;
        addToHistory(m_alphaHistory, m_alpha);
        emit alphaChanged();
        emit alphaHistoryChanged();
    }

    if (m_beta != newBeta) {
        m_beta = newBeta;
        addToHistory(m_betaHistory, m_beta);
        emit betaChanged();
        emit betaHistoryChanged();
    }

    if (m_theta != newTheta) {
        m_theta = newTheta;
        addToHistory(m_thetaHistory, m_theta);
        emit thetaChanged();
        emit thetaHistoryChanged();
    }

    // Производные метрики (зависят от Alpha/Beta/Theta)
    double newConcentration = (m_beta / (m_alpha + m_theta)) * 50.0;
    double newRelaxation = (m_alpha / (m_beta + m_theta)) * 50.0;
    double newFatigue = std::max(0.0, 100.0 - (m_alpha + m_beta) / 2.0);

    if (m_concentration != newConcentration) {
        m_concentration = newConcentration;
        emit concentrationChanged();
    }

    if (m_relaxation != newRelaxation) {
        m_relaxation = newRelaxation;
        emit relaxationChanged();
    }

    if (m_fatigue != newFatigue) {
        m_fatigue = newFatigue;
        emit fatigueChanged();
    }

    // Эмоциональные метрики
    double newFocus = 50.0 + 30.0 * std::sin(m_demoTime * 0.4);
    double newStress = 30.0 + 20.0 * std::sin(m_demoTime * 0.6);
    double newChill = 60.0 + 25.0 * std::sin(m_demoTime * 0.3);

    if (m_focus != newFocus) {
        m_focus = newFocus;
        emit focusChanged();
    }

    if (m_stress != newStress) {
        m_stress = newStress;
        emit stressChanged();
    }

    if (m_chill != newChill) {
        m_chill = newChill;
        emit chillChanged();
    }

    // Cardio метрики
    int newHeartRate = 65 + static_cast<int>(10 * std::sin(m_demoTime * 0.2));
    double newStressIndex = 30.0 + 15.0 * std::sin(m_demoTime * 0.5);

    if (m_heartRate != newHeartRate) {
        m_heartRate = newHeartRate;
        emit heartRateChanged();
    }

    if (m_stressIndex != newStressIndex) {
        m_stressIndex = newStressIndex;
        emit stressIndexChanged();
    }

    // Success rate (постепенно растет)
    double newSuccessRate = std::min(100.0, 40.0 + m_demoTime * 2.0);
    if (m_successRate != newSuccessRate) {
        m_successRate = newSuccessRate;
        emit successRateChanged();
    }
}
