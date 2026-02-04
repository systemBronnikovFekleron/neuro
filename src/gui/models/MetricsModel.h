#pragma once

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <memory>
#include <deque>

// Forward declarations
namespace Bronnikov {
    class MetricsCollector;
    struct MetricsSnapshot;
}

class DeviceController;

/**
 * @brief Qt Model для real-time отображения EEG метрик в QML
 *
 * Собирает метрики из MetricsCollector и предоставляет их QML через Q_PROPERTY.
 * Обновляется каждые 100ms для плавного отображения в UI.
 * Хранит историю последних 120 точек (2 минуты при 100ms обновлениях) для графиков.
 */
class MetricsModel : public QObject
{
    Q_OBJECT

    // Brain wave properties (Alpha/Beta/Theta)
    Q_PROPERTY(double alpha READ alpha NOTIFY alphaChanged)
    Q_PROPERTY(double beta READ beta NOTIFY betaChanged)
    Q_PROPERTY(double theta READ theta NOTIFY thetaChanged)

    // Productivity properties
    Q_PROPERTY(double concentration READ concentration NOTIFY concentrationChanged)
    Q_PROPERTY(double relaxation READ relaxation NOTIFY relaxationChanged)
    Q_PROPERTY(double fatigue READ fatigue NOTIFY fatigueChanged)

    // Emotion properties
    Q_PROPERTY(double focus READ focus NOTIFY focusChanged)
    Q_PROPERTY(double stress READ stress NOTIFY stressChanged)
    Q_PROPERTY(double chill READ chill NOTIFY chillChanged)

    // Cardio properties
    Q_PROPERTY(int heartRate READ heartRate NOTIFY heartRateChanged)
    Q_PROPERTY(double stressIndex READ stressIndex NOTIFY stressIndexChanged)

    // Success rate (calculated by Exercise)
    Q_PROPERTY(double successRate READ successRate NOTIFY successRateChanged)

    // Chart data properties (for QtCharts)
    Q_PROPERTY(QVariantList alphaHistory READ alphaHistory NOTIFY alphaHistoryChanged)
    Q_PROPERTY(QVariantList betaHistory READ betaHistory NOTIFY betaHistoryChanged)
    Q_PROPERTY(QVariantList thetaHistory READ thetaHistory NOTIFY thetaHistoryChanged)

    // Connection status
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

public:
    explicit MetricsModel(QObject* parent = nullptr);
    ~MetricsModel() override;

    // Property getters
    double alpha() const { return m_alpha; }
    double beta() const { return m_beta; }
    double theta() const { return m_theta; }
    double concentration() const { return m_concentration; }
    double relaxation() const { return m_relaxation; }
    double fatigue() const { return m_fatigue; }
    double focus() const { return m_focus; }
    double stress() const { return m_stress; }
    double chill() const { return m_chill; }
    int heartRate() const { return m_heartRate; }
    double stressIndex() const { return m_stressIndex; }
    double successRate() const { return m_successRate; }
    bool isActive() const { return m_isActive; }

    QVariantList alphaHistory() const;
    QVariantList betaHistory() const;
    QVariantList thetaHistory() const;

    // Q_INVOKABLE methods
    Q_INVOKABLE void startMonitoring();
    Q_INVOKABLE void stopMonitoring();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setMetricsCollector(void* collector);  // Accepts MetricsCollector*
    Q_INVOKABLE void setDeviceController(QObject* controller);  // НОВОЕ: для получения метрик из DeviceController
    Q_INVOKABLE void startDemoMode();  // НОВОЕ: запуск демо-режима с fake данными

signals:
    void alphaChanged();
    void betaChanged();
    void thetaChanged();
    void concentrationChanged();
    void relaxationChanged();
    void fatigueChanged();
    void focusChanged();
    void stressChanged();
    void chillChanged();
    void heartRateChanged();
    void stressIndexChanged();
    void successRateChanged();
    void alphaHistoryChanged();
    void betaHistoryChanged();
    void thetaHistoryChanged();
    void isActiveChanged();

private slots:
    void updateMetrics();

private:
    // Current metric values
    double m_alpha = 0.0;
    double m_beta = 0.0;
    double m_theta = 0.0;
    double m_concentration = 0.0;
    double m_relaxation = 0.0;
    double m_fatigue = 0.0;
    double m_focus = 0.0;
    double m_stress = 0.0;
    double m_chill = 0.0;
    int m_heartRate = 0;
    double m_stressIndex = 0.0;
    double m_successRate = 0.0;
    bool m_isActive = false;

    // History for charts (last 120 points = 2 minutes at 100ms)
    static constexpr int MAX_HISTORY_SIZE = 120;
    std::deque<double> m_alphaHistory;
    std::deque<double> m_betaHistory;
    std::deque<double> m_thetaHistory;

    // Update timer
    QTimer* m_updateTimer = nullptr;

    // Reference to C++ MetricsCollector (legacy - для прямого использования)
    Bronnikov::MetricsCollector* m_metricsCollector = nullptr;

    // НОВОЕ: Reference to DeviceController (для получения метрик через Qt)
    DeviceController* m_deviceController = nullptr;

    // НОВОЕ: Demo mode
    bool m_demoMode = false;
    double m_demoTime = 0.0;  // Время для генерации волн

    void updateFromSnapshot(const Bronnikov::MetricsSnapshot& snapshot);
    void addToHistory(std::deque<double>& history, double value);
    void generateDemoMetrics();  // НОВОЕ: генерация fake данных
};
