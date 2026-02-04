#include "MetricsCollector.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>

namespace Bronnikov {

// Глобальные маппинги для всех классификаторов (из-за отсутствия user_data в API)
static std::map<clCNFBMetricProductivity, MetricsCollector*> g_productivity_map;
static std::map<clCNFB, MetricsCollector*> g_nfb_map;
static std::map<clCEmotions, MetricsCollector*> g_emotions_map;
static std::map<clCCardio, MetricsCollector*> g_cardio_map;
static std::map<clCNFBCalibrator, MetricsCollector*> g_calibrator_map;

MetricsCollector::MetricsCollector(clCSession session)
    : m_session(session)
{
}

MetricsCollector::~MetricsCollector() {
    // Закрываем лог файл с flush
    if (m_log_file) {
        fflush(m_log_file);
        fclose(m_log_file);
        m_log_file = nullptr;
    }

    // Удаляем из глобальных маппингов
    if (m_productivity) {
        g_productivity_map.erase(m_productivity);
        clCNFBMetricsProductivity_Destroy(m_productivity);
        m_productivity = nullptr;
    }
    if (m_nfb) {
        g_nfb_map.erase(m_nfb);
        clCNFB_Destroy(m_nfb);
        m_nfb = nullptr;
    }
    if (m_emotions) {
        g_emotions_map.erase(m_emotions);
        clCEmotions_Destroy(m_emotions);
        m_emotions = nullptr;
    }
    if (m_cardio) {
        g_cardio_map.erase(m_cardio);
        clCCardio_Destroy(m_cardio);
        m_cardio = nullptr;
    }
    if (m_physiological) {
        clCPhysiologicalStates_Destroy(m_physiological);
        m_physiological = nullptr;
    }
    if (m_calibrator) {
        g_calibrator_map.erase(m_calibrator);
        // ПРИМЕЧАНИЕ: clCNFBCalibrator не имеет Destroy - управляется автоматически
        m_calibrator = nullptr;
    }
}

bool MetricsCollector::initialize() {
    if (!m_session) {
        std::cerr << "[MetricsCollector] Ошибка: сессия не установлена" << std::endl;
        return false;
    }

    clCError error = clC_Error_OK;

    // Создание CNFBMetricProductivity (основной классификатор)
    m_productivity = clCNFBMetricsProductivity_Create(m_session, "", 0.05, 0.05, 0.001, &error);
    if (!m_productivity || error != clC_Error_OK) {
        std::cerr << "[MetricsCollector] ✗ Не удалось создать CNFBMetricProductivity (error: "
                  << error << ")" << std::endl;
        return false;
    }

    // Регистрируем в глобальном маппинге
    g_productivity_map[m_productivity] = this;

    // Установка callback для получения метрик продуктивности
    clCNFBMetricsProductivityValuesDelegate productivity_delegate =
        clCNFBMetricsProductivity_GetOnProductivityValuesEvent(m_productivity);
    clCNFBMetricsProductivity_ValuesEvent_Set(productivity_delegate, onProductivityUpdateCallback);

    // Инициализация NFB
    error = clC_Error_OK;
    clCNFBMetricsProductivity_InitializeNFB(m_productivity, "", &error);
    if (error != clC_Error_OK) {
        std::cerr << "[MetricsCollector] ✗ Ошибка инициализации NFB (error: "
                  << error << ")" << std::endl;
        return false;
    }

    // Создание CNFB для отслеживания ритмов (Alpha/Beta/Theta)
    m_nfb = clCNFB_Create(m_session);
    if (m_nfb) {
        g_nfb_map[m_nfb] = this;

        clCNFBDelegateNFBUserState nfb_delegate = clCNFB_GetOnUserStateChangedEvent(m_nfb);
        clCNFBDelegateNFBUserState_Set(nfb_delegate, onNFBUpdateCallback);

        // Установка callbacks для событий инициализации и обучения
        clCNFBDelegate initialized_delegate = clCNFB_GetOnInitializedEvent(m_nfb);
        clCNFBDelegate_Set(initialized_delegate, onNFBInitializedCallback);

        clCNFBDelegate trained_delegate = clCNFB_GetOnModelTrainedEvent(m_nfb);
        clCNFBDelegate_Set(trained_delegate, onNFBModelTrainedCallback);

        // Запуск инициализации (цепочка: OnInitialized → Train → OnModelTrained → AddFeedbackFunction)
        clCNFBCallResult nfb_result = clCNFB_Initialize(m_nfb);
        if (nfb_result != clC_NFB_Success) {
            std::cerr << "[MetricsCollector] ⚠ CNFB инициализация вернула: " << nfb_result << std::endl;
        }
    }

    // Создание CEmotions
    m_emotions = clCEmotions_Create(m_session, 0.5, 1.0);
    if (m_emotions) {
        g_emotions_map[m_emotions] = this;

        // Callback для обновления метрик эмоций
        clCEmotionsDelegateEmotionalStatesUpdate emotions_delegate =
            clCEmotions_GetOnEmotionalStatesUpdateEvent(m_emotions);
        clCEmotionsDelegateEmotionalStatesUpdate_Set(emotions_delegate, onEmotionsUpdateCallback);

        // КРИТИЧНО: Callbacks для инициализации и калибровки
        clCEmotionsDelegate initialized_delegate = clCEmotions_GetOnInitializedEvent(m_emotions);
        clCEmotionsDelegate_Set(initialized_delegate, onEmotionsInitializedCallback);

        clCEmotionsDelegate calibrated_delegate = clCEmotions_GetOnCalibratedEvent(m_emotions);
        clCEmotionsDelegate_Set(calibrated_delegate, onEmotionsCalibratedCallback);

        clCEmotionsDelegateString error_delegate = clCEmotions_GetOnErrorEvent(m_emotions);
        clCEmotionsDelegateString_Set(error_delegate, onEmotionsErrorCallback);

        // КРИТИЧНО: Инициализация CEmotions (как CNFB требует Initialize)
        std::cout << "[MetricsCollector] Инициализация CEmotions..." << std::endl;
        clCEmotions_Initialize(m_emotions, "");  // platformAddress пока пустой
    }

    // Создание CCardio
    m_cardio = clCCardio_Create(m_session);
    if (m_cardio) {
        g_cardio_map[m_cardio] = this;

        clCCardioIndexesDelegate cardio_delegate = clCCardio_GetOnIndexesUpdateEvent(m_cardio);
        clCCardioDelegateIndexesUpdate_Set(cardio_delegate, onCardioUpdateCallback);

        clCCardio_Initialize(m_cardio);
    }

    // Создание CPhysiologicalStates
    m_physiological = clCPhysiologicalStates_Create(m_session);

    // Создание калибратора NFB
    m_calibrator = clCNFBCalibrator_CreateOrGet(m_session);
    if (m_calibrator) {
        g_calibrator_map[m_calibrator] = this;

        // Устанавливаем callbacks калибратора
        clCNFBCalibratorDelegateIndividualNFBCalibrated calibrated_delegate =
            clCNFBCalibrator_GetOnIndividualNFBCalibratedEvent(m_calibrator);
        clCNFBCalibratorDelegateIndividualNFBCalibrated_Set(calibrated_delegate,
                                                             onCalibrationCompleteCallback);

        clCNFBCalibratorDelegateReadyToCalibrate ready_delegate =
            clCNFBCalibrator_GetOnReadyToCalibrate(m_calibrator);
        clCNFBCalibratorDelegateReadyToCalibrate_Set(ready_delegate,
                                                      onCalibratorReadyCallback);
    }

    return true;
}

bool MetricsCollector::startCalibration(int duration_seconds) {
    if (!m_session) {
        std::cerr << "[MetricsCollector] ✗ Калибровка: сессия не установлена" << std::endl;
        return false;
    }

    if (!m_calibrator) {
        m_calibrator = clCNFBCalibrator_CreateOrGet(m_session);
        if (!m_calibrator) {
            std::cerr << "[MetricsCollector] ✗ Не удалось создать калибратор" << std::endl;
            return false;
        }

        // Регистрируем в глобальном маппинге
        g_calibrator_map[m_calibrator] = this;

        // Используем callback для завершения калибровки
        clCNFBCalibratorDelegateIndividualNFBCalibrated calibrator_delegate =
            clCNFBCalibrator_GetOnIndividualNFBCalibratedEvent(m_calibrator);
        clCNFBCalibratorDelegateIndividualNFBCalibrated_Set(calibrator_delegate,
                                                             onCalibrationCompleteCallback);
    }

    // Используем быструю калибровку
    clCError error = clC_Error_OK;
    m_calibrating = true;
    clCNFBCalibrator_CalibrateIndividualNFBQuick(m_calibrator, &error);

    if (error != clC_Error_OK) {
        std::cerr << "[MetricsCollector] ✗ Ошибка запуска калибровки (error: "
                  << error << ")" << std::endl;
        m_calibrating = false;
        return false;
    }

    std::cout << "[MetricsCollector] ✓ Калибровка запущена" << std::endl;
    return true;
}

bool MetricsCollector::isCalibrating() const {
    return m_calibrating;
}

bool MetricsCollector::isCalibrated() const {
    return m_calibrated;
}

MetricsSnapshot MetricsCollector::getCurrentMetrics() const {
    return m_current_metrics;
}

void MetricsCollector::enableLogging(bool enabled, const std::string& file_path) {
    m_logging_enabled = enabled;

    if (enabled) {
        if (m_log_file) {
            fclose(m_log_file);
        }

        // Создание имени файла с timestamp, если не указано
        if (file_path.empty()) {
            auto now = std::chrono::system_clock::now();
            auto time_t_now = std::chrono::system_clock::to_time_t(now);
            std::tm tm_now;
            localtime_r(&time_t_now, &tm_now);

            char timestamp_buffer[64];
            std::strftime(timestamp_buffer, sizeof(timestamp_buffer), "%Y%m%d_%H%M%S", &tm_now);
            m_log_file_path = "session_" + std::string(timestamp_buffer) + ".csv";
        } else {
            m_log_file_path = file_path;
        }

        m_log_file = fopen(m_log_file_path.c_str(), "w");

        if (m_log_file) {
            // Запись заголовков CSV
            fprintf(m_log_file,
                "timestamp,alpha_power,beta_power,theta_power,alpha_beta_ratio,"
                "concentration,relaxation,fatigue,gravity,"
                "focus,chill,stress,anger,self_control,"
                "heart_rate,stress_index,iaf\n");
            fflush(m_log_file);
        }
    } else {
        if (m_log_file) {
            fclose(m_log_file);
            m_log_file = nullptr;
        }
    }
}

void MetricsCollector::saveBaseline() {
    m_baseline = m_current_metrics;
}

MetricsSnapshot MetricsCollector::getDeviationFromBaseline() const {
    MetricsSnapshot deviation;

    if (m_baseline.timestamp == 0.0) {
        return deviation;  // Baseline не установлен
    }

    // Вычисляем процентные изменения
    auto calc_percent_change = [](double current, double baseline) -> double {
        if (baseline == 0.0) return 0.0;
        return ((current - baseline) / baseline) * 100.0;
    };

    deviation.alpha_power = calc_percent_change(m_current_metrics.alpha_power, m_baseline.alpha_power);
    deviation.beta_power = calc_percent_change(m_current_metrics.beta_power, m_baseline.beta_power);
    deviation.theta_power = calc_percent_change(m_current_metrics.theta_power, m_baseline.theta_power);
    deviation.concentration = calc_percent_change(m_current_metrics.concentration, m_baseline.concentration);
    deviation.relaxation = calc_percent_change(m_current_metrics.relaxation, m_baseline.relaxation);
    deviation.fatigue = calc_percent_change(m_current_metrics.fatigue, m_baseline.fatigue);
    deviation.focus = calc_percent_change(m_current_metrics.focus, m_baseline.focus);
    deviation.stress = calc_percent_change(m_current_metrics.stress, m_baseline.stress);

    return deviation;
}

void MetricsCollector::updateProductivityMetrics(const clCNFBMetricsProductivityValues* values) {
    if (!values) return;

    m_current_metrics.concentration = values->concentrationScore;
    m_current_metrics.relaxation = values->relaxationScore;
    m_current_metrics.fatigue = values->fatigueScore;
    m_current_metrics.gravity = values->gravityScore;

    // DEBUG: Логируем получение данных
    static int prod_count = 0;
    if (++prod_count % 20 == 0) {
        std::cout << "[DEBUG] Productivity data #" << prod_count
                  << " | Concentration: " << values->concentrationScore
                  << " | Relaxation: " << values->relaxationScore << std::endl;
    }
}

void MetricsCollector::updateNFBMetrics(const clCNFBUserState* state) {
    if (!state || !state->feedbackData) return;

    // Парсим данные feedback функций
    // Предполагаем что данные идут в порядке добавления: Alpha, Beta, Theta
    if (state->feedbackSize >= 1) {
        m_current_metrics.alpha_power = state->feedbackData[0];
    }
    if (state->feedbackSize >= 2) {
        m_current_metrics.beta_power = state->feedbackData[1];
    }
    if (state->feedbackSize >= 3) {
        m_current_metrics.theta_power = state->feedbackData[2];
    }

    if (m_current_metrics.beta_power > 0.0) {
        m_current_metrics.alpha_beta_ratio =
            m_current_metrics.alpha_power / m_current_metrics.beta_power;
    }

    // DEBUG: Логируем получение данных
    static int nfb_count = 0;
    if (++nfb_count % 20 == 0) {
        std::cout << "[DEBUG] NFB data #" << nfb_count
                  << " | Alpha: " << m_current_metrics.alpha_power
                  << " | Beta: " << m_current_metrics.beta_power
                  << " | Theta: " << m_current_metrics.theta_power << std::endl;
    }
}

void MetricsCollector::updateEmotionMetrics(const clCEmotionalStates* states) {
    if (!states) return;

    m_current_metrics.focus = states->focus;
    m_current_metrics.chill = states->chill;
    m_current_metrics.stress = states->stress;
    m_current_metrics.anger = states->anger;
    m_current_metrics.self_control = states->selfControl;
}

void MetricsCollector::updateCardioMetrics(const clCCardioData& data) {
    m_current_metrics.heart_rate = data.heartRate;
    m_current_metrics.stress_index = data.stressIndex;

    // DEBUG: Логируем получение данных
    static int cardio_count = 0;
    if (++cardio_count % 10 == 0) {
        std::cout << "[DEBUG] Cardio data #" << cardio_count
                  << " | HR: " << data.heartRate
                  << " | Stress Index: " << data.stressIndex << std::endl;
    }
}

void MetricsCollector::writeToLog(const MetricsSnapshot& metrics) {
    if (!m_log_file) return;

    fprintf(m_log_file,
        "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
        metrics.timestamp,
        metrics.alpha_power, metrics.beta_power, metrics.theta_power, metrics.alpha_beta_ratio,
        metrics.concentration, metrics.relaxation, metrics.fatigue, metrics.gravity,
        metrics.focus, metrics.chill, metrics.stress, metrics.anger, metrics.self_control,
        metrics.heart_rate, metrics.stress_index, metrics.iaf);

    fflush(m_log_file);
}

// Static callbacks
void MetricsCollector::onProductivityUpdateCallback(clCNFBMetricProductivity productivity,
                                                     const clCNFBMetricsProductivityValues* values) {
    auto it = g_productivity_map.find(productivity);
    if (it == g_productivity_map.end()) {
        return;  // Коллектор не найден
    }

    MetricsCollector* collector = it->second;
    collector->updateProductivityMetrics(values);

    collector->m_current_metrics.timestamp = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    if (collector->m_logging_enabled) {
        collector->writeToLog(collector->m_current_metrics);
    }

    if (collector->m_on_metrics_update) {
        collector->m_on_metrics_update(collector->m_current_metrics);
    }
}

void MetricsCollector::onNFBUpdateCallback(clCNFB nfb, const clCNFBUserState* state) {
    auto it = g_nfb_map.find(nfb);
    if (it == g_nfb_map.end()) {
        return;  // Коллектор не найден
    }

    MetricsCollector* collector = it->second;
    collector->updateNFBMetrics(state);
}

void MetricsCollector::onEmotionsUpdateCallback(clCEmotions emotions, const clCEmotionalStates* states) {
    auto it = g_emotions_map.find(emotions);
    if (it == g_emotions_map.end()) {
        return;  // Коллектор не найден
    }

    MetricsCollector* collector = it->second;
    collector->updateEmotionMetrics(states);

    // DEBUG: Логируем получение эмоций
    static int emotions_callback_count = 0;
    if (++emotions_callback_count % 10 == 0) {  // Каждое 10-е обновление
        std::cout << "[MetricsCollector] CEmotions callback #" << emotions_callback_count
                  << " | Focus: " << states->focus
                  << " | Stress: " << states->stress
                  << " | Chill: " << states->chill << std::endl;
    }
}

void MetricsCollector::onEmotionsInitializedCallback(clCEmotions emotions) {
    std::cout << "[MetricsCollector] >>> CEmotions INITIALIZED! <<<" << std::endl;

    auto it = g_emotions_map.find(emotions);
    if (it != g_emotions_map.end() && it->second) {
        MetricsCollector* collector = it->second;
        collector->m_emotions_initialized = true;

        // КРИТИЧНО: После инициализации запускаем калибровку
        std::cout << "[MetricsCollector] Запуск калибровки CEmotions..." << std::endl;
        clCEmotions_StartCalibration(emotions);
    }
}

void MetricsCollector::onEmotionsCalibratedCallback(clCEmotions emotions) {
    std::cout << "[MetricsCollector] >>> CEmotions CALIBRATED! <<<" << std::endl;

    auto it = g_emotions_map.find(emotions);
    if (it != g_emotions_map.end() && it->second) {
        MetricsCollector* collector = it->second;
        collector->m_emotions_calibrated = true;

        std::cout << "[MetricsCollector] ✓ CEmotions готов к работе!" << std::endl;
        std::cout << "[MetricsCollector]   Метрики эмоций должны начать поступать" << std::endl;
    }
}

void MetricsCollector::onEmotionsErrorCallback(clCEmotions emotions, const char* error) {
    std::cerr << "[MetricsCollector] ✗ CEmotions ERROR: " << (error ? error : "unknown") << std::endl;
}

void MetricsCollector::onCardioUpdateCallback(clCCardio cardio, clCCardioData data) {
    auto it = g_cardio_map.find(cardio);
    if (it == g_cardio_map.end()) {
        return;  // Коллектор не найден
    }

    MetricsCollector* collector = it->second;
    collector->updateCardioMetrics(data);
}

void MetricsCollector::onNFBInitializedCallback(clCNFB nfb) {
    std::cout << "[MetricsCollector] >>> CNFB INITIALIZED! <<<" << std::endl;

    auto it = g_nfb_map.find(nfb);
    if (it != g_nfb_map.end() && it->second) {
        MetricsCollector* collector = it->second;
        collector->m_nfb_initialized = true;

        // КРИТИЧНО: После инициализации вызываем Train (как в примере CapsuleAPI)
        std::cout << "[MetricsCollector] Запуск обучения модели CNFB..." << std::endl;
        clCNFBCallResult result = clCNFB_Train(nfb);

        if (result == clC_NFB_Success) {
            std::cout << "[MetricsCollector] ✓ Обучение CNFB запущено (ждем OnModelTrained)" << std::endl;
        } else {
            std::cerr << "[MetricsCollector] ✗ Ошибка запуска обучения CNFB: " << result << std::endl;
        }
    }
}

void MetricsCollector::onNFBModelTrainedCallback(clCNFB nfb) {
    std::cout << "[MetricsCollector] >>> CNFB MODEL TRAINED! <<<" << std::endl;

    auto it = g_nfb_map.find(nfb);
    if (it != g_nfb_map.end() && it->second) {
        MetricsCollector* collector = it->second;
        collector->m_nfb_trained = true;

        // КРИТИЧНО: ТОЛЬКО ПОСЛЕ обучения добавляем feedback функции (как в примере CapsuleAPI)
        std::cout << "[MetricsCollector] Добавление feedback функций (alpha, beta, theta)..." << std::endl;

        clCNFBCallResult alpha_result = clCNFB_AddFeedbackFunction(nfb, "alpha");
        clCNFBCallResult beta_result = clCNFB_AddFeedbackFunction(nfb, "beta");
        clCNFBCallResult theta_result = clCNFB_AddFeedbackFunction(nfb, "theta");

        if (alpha_result == clC_NFB_Success && beta_result == clC_NFB_Success && theta_result == clC_NFB_Success) {
            std::cout << "[MetricsCollector] ✓ Все feedback функции добавлены успешно!" << std::endl;
            std::cout << "[MetricsCollector]   Alpha/Beta/Theta метрики должны начать поступать" << std::endl;
        } else {
            std::cerr << "[MetricsCollector] ✗ Ошибка добавления feedback функций" << std::endl;
            std::cerr << "[MetricsCollector]   alpha: " << alpha_result
                      << ", beta: " << beta_result
                      << ", theta: " << theta_result << std::endl;
        }
    }
}

void MetricsCollector::onCalibratorReadyCallback(clCNFBCalibrator calibrator) {
    std::cout << "[MetricsCollector] >>> CALIBRATOR READY EVENT! <<<" << std::endl;

    auto it = g_calibrator_map.find(calibrator);
    if (it != g_calibrator_map.end()) {
        MetricsCollector* collector = it->second;
        collector->m_calibrator_ready = true;

        // Автоматический запуск быстрой калибровки (необходимо для работы NFB callbacks)
        std::cout << "[MetricsCollector] Автокалибровка запущена (~30 сек)" << std::endl;
        clCError error = clC_Error_OK;
        clCNFBCalibrator_CalibrateIndividualNFBQuick(calibrator, &error);
        if (error == clC_Error_OK) {
            collector->m_calibrating = true;
        } else {
            std::cerr << "[MetricsCollector] ✗ Ошибка автокалибровки" << std::endl;
        }
    }
}

void MetricsCollector::onCalibrationCompleteCallback(clCNFBCalibrator calibrator,
                                                       const clCIndividualNFBData* data,
                                                       clCIndividualNFBCalibrationFailReason reason) {
    auto it = g_calibrator_map.find(calibrator);
    if (it == g_calibrator_map.end()) {
        return;  // Коллектор не найден
    }

    MetricsCollector* collector = it->second;
    bool success = (reason == clC_IndividualNFBCalibrationFailReason_None);

    // Сбрасываем флаг калибровки
    collector->m_calibrating = false;

    if (success && data) {
        collector->m_calibrated = true;
        collector->m_current_metrics.iaf = data->individualFrequency;

        if (collector->m_on_calibration_complete) {
            collector->m_on_calibration_complete(true, data->individualFrequency);
        }
    } else {
        if (collector->m_on_calibration_complete) {
            collector->m_on_calibration_complete(false, 0.0);
        }
    }
}

} // namespace Bronnikov
