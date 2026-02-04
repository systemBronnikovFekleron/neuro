#pragma once

#include <CClientAPI.h>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <map>

namespace Bronnikov {

/**
 * @brief Снимок метрик в конкретный момент времени
 */
struct MetricsSnapshot {
    double timestamp = 0.0;

    // Базовые ритмы мозга
    double alpha_power = 0.0;      // 8-12 Hz
    double beta_power = 0.0;       // 13-30 Hz
    double theta_power = 0.0;      // 4-8 Hz
    double alpha_beta_ratio = 0.0;

    // Метрики продуктивности (CNFBMetricsProductivity)
    double concentration = 0.0;    // 0-100
    double relaxation = 0.0;       // 0-100
    double fatigue = 0.0;          // 0-100
    double gravity = 0.0;          // гравитационная компонента

    // Эмоции (CEmotions)
    double focus = 0.0;            // 0-100
    double chill = 0.0;            // спокойствие
    double stress = 0.0;           // 0-100
    double anger = 0.0;            // 0-100
    double self_control = 0.0;     // 0-100

    // Кардио (CCardio)
    double heart_rate = 0.0;       // уд/мин
    double stress_index = 0.0;     // индекс стресса

    // Индивидуальная альфа-частота
    double iaf = 0.0;              // Individual Alpha Frequency
};

/**
 * @brief Коллектор метрик с классификаторов CapsuleAPI
 *
 * Собирает данные со всех классификаторов (NFB, Emotions, Cardio)
 * и предоставляет унифицированный интерфейс для их получения.
 */
class MetricsCollector {
public:
    MetricsCollector(clCSession session);
    ~MetricsCollector();

    // Запрет копирования
    MetricsCollector(const MetricsCollector&) = delete;
    MetricsCollector& operator=(const MetricsCollector&) = delete;

    /**
     * @brief Инициализация всех классификаторов
     * @return true если инициализация успешна
     */
    bool initialize();

    /**
     * @brief Запуск калибровки NFB
     *
     * Калибровка определяет индивидуальные параметры мозга пользователя
     * для более точной работы классификаторов.
     *
     * @param duration_seconds Длительность калибровки (рекомендуется 60-120 сек)
     * @return true если калибровка запущена успешно
     */
    bool startCalibration(int duration_seconds = 90);

    /**
     * @brief Проверка статуса калибровки
     */
    bool isCalibrating() const;
    bool isCalibrated() const;

    /**
     * @brief Получение текущего снимка всех метрик
     */
    MetricsSnapshot getCurrentMetrics() const;

    /**
     * @brief Включение/выключение логирования в CSV
     * @param enabled Включить логирование
     * @param file_path Путь к CSV файлу
     */
    void enableLogging(bool enabled, const std::string& file_path = "");

    /**
     * @brief Сохранение текущего состояния как baseline
     *
     * Baseline используется для сравнения изменений метрик
     * относительно начального состояния пользователя.
     */
    void saveBaseline();

    /**
     * @brief Получение baseline метрик
     */
    MetricsSnapshot getBaseline() const { return m_baseline; }

    /**
     * @brief Вычисление отклонения текущих метрик от baseline
     * @return Снимок с относительными изменениями (в процентах)
     */
    MetricsSnapshot getDeviationFromBaseline() const;

    // Callbacks для real-time уведомлений
    using MetricsCallback = std::function<void(const MetricsSnapshot& metrics)>;
    using CalibrationCallback = std::function<void(bool success, double iaf)>;

    void setOnMetricsUpdateCallback(MetricsCallback callback) { m_on_metrics_update = callback; }
    void setOnCalibrationCompleteCallback(CalibrationCallback callback) { m_on_calibration_complete = callback; }

private:
    clCSession m_session = nullptr;

    // Классификаторы
    clCNFBMetricProductivity m_productivity = nullptr;
    clCNFB m_nfb = nullptr;
    clCEmotions m_emotions = nullptr;
    clCCardio m_cardio = nullptr;
    clCPhysiologicalStates m_physiological = nullptr;
    clCNFBCalibrator m_calibrator = nullptr;

    MetricsSnapshot m_current_metrics;
    MetricsSnapshot m_baseline;

    bool m_calibrated = false;
    bool m_calibrating = false;  // Флаг процесса калибровки
    bool m_calibrator_ready = false;  // Флаг готовности калибратора (КРИТИЧНО для NFB callbacks!)
    bool m_nfb_initialized = false;  // Флаг инициализации CNFB
    bool m_nfb_trained = false;  // Флаг обучения модели CNFB
    bool m_emotions_initialized = false;  // Флаг инициализации CEmotions
    bool m_emotions_calibrated = false;  // Флаг калибровки CEmotions
    bool m_logging_enabled = false;
    std::string m_log_file_path;
    FILE* m_log_file = nullptr;

    MetricsCallback m_on_metrics_update;
    CalibrationCallback m_on_calibration_complete;

    // Внутренние методы обновления
    void updateProductivityMetrics(const clCNFBMetricsProductivityValues* values);
    void updateNFBMetrics(const clCNFBUserState* state);
    void updateEmotionMetrics(const clCEmotionalStates* states);
    void updateCardioMetrics(const clCCardioData& data);
    void writeToLog(const MetricsSnapshot& metrics);

    // Static callbacks для C API
    static void onProductivityUpdateCallback(clCNFBMetricProductivity productivity,
                                             const clCNFBMetricsProductivityValues* values);
    static void onNFBUpdateCallback(clCNFB nfb, const clCNFBUserState* state);
    static void onEmotionsUpdateCallback(clCEmotions emotions, const clCEmotionalStates* states);
    static void onEmotionsInitializedCallback(clCEmotions emotions);
    static void onEmotionsCalibratedCallback(clCEmotions emotions);
    static void onEmotionsErrorCallback(clCEmotions emotions, const char* error);
    static void onCardioUpdateCallback(clCCardio cardio, clCCardioData data);
    static void onCalibrationCompleteCallback(clCNFBCalibrator calibrator,
                                               const clCIndividualNFBData* data,
                                               clCIndividualNFBCalibrationFailReason reason);
    static void onCalibratorReadyCallback(clCNFBCalibrator calibrator);
    static void onNFBInitializedCallback(clCNFB nfb);
    static void onNFBModelTrainedCallback(clCNFB nfb);
};

} // namespace Bronnikov
