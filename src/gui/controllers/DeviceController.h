#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QTimer>
#include <memory>

// Forward declarations
namespace Bronnikov {
    class CapsuleManager;
    class SessionManager;
    class MetricsCollector;
    struct MetricsSnapshot;
}

/**
 * @brief Qt Controller для управления подключением к Neiry устройству
 *
 * Предоставляет QML интерфейс для:
 * - Обнаружения устройств
 * - Подключения/отключения
 * - Запуска/остановки EEG сессии
 * - Калибровки NFB
 * - Мониторинга статуса устройства (батарея, качество сигнала)
 */
class DeviceController : public QObject
{
    Q_OBJECT

    // Properties для QML
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(bool isSessionActive READ isSessionActive NOTIFY isSessionActiveChanged)
    Q_PROPERTY(QString deviceName READ deviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(int signalQualityChannel1 READ signalQualityChannel1 NOTIFY signalQualityChanged)
    Q_PROPERTY(int signalQualityChannel2 READ signalQualityChannel2 NOTIFY signalQualityChanged)

    // Calibration properties
    Q_PROPERTY(bool isCalibrating READ isCalibrating NOTIFY isCalibratingChanged)
    Q_PROPERTY(int calibrationProgress READ calibrationProgress NOTIFY calibrationProgressChanged)
    Q_PROPERTY(double iaf READ iaf NOTIFY iafChanged)
    Q_PROPERTY(double iapf READ iapf NOTIFY iapfChanged)

public:
    explicit DeviceController(QObject* parent = nullptr);
    ~DeviceController() override;

    // Property getters
    bool isConnected() const { return m_isConnected; }
    bool isSessionActive() const { return m_isSessionActive; }
    QString deviceName() const { return m_deviceName; }
    int batteryLevel() const { return m_batteryLevel; }
    QString connectionStatus() const { return m_connectionStatus; }
    int signalQualityChannel1() const { return m_signalQualityChannel1; }
    int signalQualityChannel2() const { return m_signalQualityChannel2; }
    bool isCalibrating() const { return m_isCalibrating; }
    int calibrationProgress() const { return m_calibrationProgress; }
    double iaf() const { return m_iaf; }
    double iapf() const { return m_iapf; }

    // Setters для C++ интеграции
    void setCapsuleManager(Bronnikov::CapsuleManager* manager);

    // Q_INVOKABLE methods для вызова из QML
    Q_INVOKABLE void discoverDevices();
    Q_INVOKABLE void connectToDevice(const QString& deviceId = "");
    Q_INVOKABLE void disconnectFromDevice();

    Q_INVOKABLE void startSession();
    Q_INVOKABLE void stopSession();

    Q_INVOKABLE void startCalibration();
    Q_INVOKABLE void cancelCalibration();

    Q_INVOKABLE QVariantMap getDeviceInfo() const;
    Q_INVOKABLE QVariantMap getCurrentMetrics() const;

signals:
    // Property change signals
    void isConnectedChanged();
    void isSessionActiveChanged();
    void deviceNameChanged();
    void batteryLevelChanged();
    void connectionStatusChanged();
    void signalQualityChanged();
    void isCalibratingChanged();
    void calibrationProgressChanged();
    void iafChanged();
    void iapfChanged();

    // Event signals
    void deviceDiscovered(const QString& deviceId, const QString& deviceName);
    void connectionFailed(const QString& error);
    void sessionStarted();
    void sessionStopped();
    void calibrationCompleted(double iaf, double iapf);
    void calibrationFailed(const QString& error);
    void errorOccurred(const QString& error);

    // НОВОЕ: Сигналы прогресса для плавного UI
    void searchProgress(const QString& message);  // "Поиск устройств..."
    void connectionProgress(const QString& message);  // "Подключение к Capsule...", "Подключение к устройству..."

private slots:
    // Slots для CapsuleAPI callbacks (через сигналы из менеджеров)
    void onDeviceConnected();
    void onDeviceDisconnected();
    void onSessionStateChanged();
    void onBatteryLevelChanged(int level);
    void onSignalQualityChanged(int ch1, int ch2);
    void onCalibrationProgressChanged(int progress);
    void onCalibrationCompleted(double iaf, double iapf);

    // НОВОЕ: Слоты для асинхронных операций
    void onAsyncUpdate();  // Вызывается таймером для update() без блокировки UI

private:
    // Properties state
    bool m_isConnected = false;
    bool m_isSessionActive = false;
    QString m_deviceName;
    int m_batteryLevel = 0;
    QString m_connectionStatus = "Disconnected";
    int m_signalQualityChannel1 = 0;
    int m_signalQualityChannel2 = 0;
    bool m_isCalibrating = false;
    int m_calibrationProgress = 0;
    double m_iaf = 0.0;
    double m_iapf = 0.0;

    // C++ business logic references
    Bronnikov::CapsuleManager* m_capsuleManager = nullptr;
    std::unique_ptr<Bronnikov::SessionManager> m_sessionManager;
    std::unique_ptr<Bronnikov::MetricsCollector> m_metricsCollector;

    // НОВОЕ: Асинхронные операции
    QTimer* m_asyncTimer = nullptr;

    enum class AsyncOperation {
        None,
        ConnectingToBackend,
        DiscoveringDevices,
        ConnectingToDevice,
        WaitingForDeviceData,
        StartingSession,
        WaitingForSessionStart
    };

    AsyncOperation m_currentOperation = AsyncOperation::None;
    int m_operationStep = 0;  // Счетчик шагов текущей операции
    QString m_targetDeviceId;  // ID устройства для подключения
    bool m_wantAutoConnect = false;  // Флаг: подключиться к первому найденному устройству

    // Internal helpers
    void updateConnectionStatus();
    void setupCallbacks();
    void startAsyncOperation(AsyncOperation operation);
    void stopAsyncOperation();
};
