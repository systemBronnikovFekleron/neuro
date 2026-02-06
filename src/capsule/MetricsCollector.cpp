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
static std::map<clCSession, MetricsCollector*> g_session_map;

MetricsCollector::MetricsCollector(clCSession session)
    : m_session(session)
{
    // Регистрируем сессию для Research Mode callbacks
    g_session_map[session] = this;
}

MetricsCollector::~MetricsCollector() {
    // Закрываем лог файл с flush
    if (m_log_file) {
        fflush(m_log_file);
        fclose(m_log_file);
        m_log_file = nullptr;
    }

    // Закрываем Research Mode файл
    if (m_eeg_raw_file) {
        fclose(m_eeg_raw_file);
        m_eeg_raw_file = nullptr;
    }

    // Удаляем из session map
    g_session_map.erase(m_session);

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
    std::cout << "[MetricsCollector] 🎭 Создание CEmotions (speed=0.5, maxSpeed=1.0)..." << std::endl;
    m_emotions = clCEmotions_Create(m_session, 0.5, 1.0);
    if (m_emotions) {
        std::cout << "[MetricsCollector] ✓ CEmotions создан, handle=" << m_emotions << std::endl;
        g_emotions_map[m_emotions] = this;

        // Callback для обновления метрик эмоций
        clCEmotionsDelegateEmotionalStatesUpdate emotions_delegate =
            clCEmotions_GetOnEmotionalStatesUpdateEvent(m_emotions);
        clCEmotionsDelegateEmotionalStatesUpdate_Set(emotions_delegate, onEmotionsUpdateCallback);
        std::cout << "[MetricsCollector]   ✓ OnEmotionalStatesUpdate callback установлен" << std::endl;

        // КРИТИЧНО: Callbacks для инициализации и калибровки
        clCEmotionsDelegate initialized_delegate = clCEmotions_GetOnInitializedEvent(m_emotions);
        clCEmotionsDelegate_Set(initialized_delegate, onEmotionsInitializedCallback);
        std::cout << "[MetricsCollector]   ✓ OnInitialized callback установлен" << std::endl;

        clCEmotionsDelegate calibrated_delegate = clCEmotions_GetOnCalibratedEvent(m_emotions);
        clCEmotionsDelegate_Set(calibrated_delegate, onEmotionsCalibratedCallback);
        std::cout << "[MetricsCollector]   ✓ OnCalibrated callback установлен" << std::endl;

        clCEmotionsDelegateString error_delegate = clCEmotions_GetOnErrorEvent(m_emotions);
        clCEmotionsDelegateString_Set(error_delegate, onEmotionsErrorCallback);
        std::cout << "[MetricsCollector]   ✓ OnError callback установлен" << std::endl;

        // КРИТИЧНО: Инициализация CEmotions (как CNFB требует Initialize)
        std::cout << "[MetricsCollector] 🎭 Запуск инициализации CEmotions..." << std::endl;
        std::cout << "[MetricsCollector]   (Ожидаем событие OnInitialized, затем автоматическая калибровка)" << std::endl;
        clCEmotions_Initialize(m_emotions, "");  // platformAddress пока пустой
    } else {
        std::cerr << "[MetricsCollector] ❌ ОШИБКА: clCEmotions_Create вернул nullptr!" << std::endl;
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
    std::cout << "[MetricsCollector] 🎯 Создание NFB Calibrator..." << std::endl;
    m_calibrator = clCNFBCalibrator_CreateOrGet(m_session);
    if (m_calibrator) {
        std::cout << "[MetricsCollector] ✓ NFB Calibrator создан, handle=" << m_calibrator << std::endl;
        g_calibrator_map[m_calibrator] = this;

        // Устанавливаем callbacks калибратора
        clCNFBCalibratorDelegateIndividualNFBCalibrated calibrated_delegate =
            clCNFBCalibrator_GetOnIndividualNFBCalibratedEvent(m_calibrator);
        clCNFBCalibratorDelegateIndividualNFBCalibrated_Set(calibrated_delegate,
                                                             onCalibrationCompleteCallback);
        std::cout << "[MetricsCollector]   ✓ OnIndividualNFBCalibrated callback установлен" << std::endl;

        clCNFBCalibratorDelegateReadyToCalibrate ready_delegate =
            clCNFBCalibrator_GetOnReadyToCalibrate(m_calibrator);
        clCNFBCalibratorDelegateReadyToCalibrate_Set(ready_delegate,
                                                      onCalibratorReadyCallback);
        std::cout << "[MetricsCollector]   ✓ OnReadyToCalibrate callback установлен" << std::endl;
        std::cout << "[MetricsCollector]   (Ожидаем событие OnReadyToCalibrate...)" << std::endl;
    } else {
        std::cerr << "[MetricsCollector] ❌ ОШИБКА: clCNFBCalibrator_CreateOrGet вернул nullptr!" << std::endl;
    }

    return true;
}

bool MetricsCollector::startCalibration(int duration_seconds) {
    if (!m_session) {
        std::cerr << "[MetricsCollector] ✗ Калибровка: сессия не установлена" << std::endl;
        return false;
    }

    if (!m_calibrator) {
        std::cerr << "[MetricsCollector] ✗ Калибровка: калибратор не создан (ошибка в initialize)" << std::endl;
        return false;
    }

    // ВАЖНО: Проверяем, готов ли калибратор
    // Событие OnReadyToCalibrate должно произойти автоматически после инициализации
    // Если оно еще не произошло, нужно подождать
    if (!m_calibrator_ready) {
        std::cout << "[MetricsCollector] ⏳ Калибратор еще не готов, ожидаем события OnReadyToCalibrate..." << std::endl;
        std::cout << "[MetricsCollector]    Попробуйте запустить калибровку через несколько секунд" << std::endl;
        return false;
    }

    // ПРИМЕЧАНИЕ: Фактический запуск калибровки происходит в onCalibratorReadyCallback()
    // Здесь мы только повторно запускаем, если калибратор уже готов и калибровка не идет
    if (m_calibrating) {
        std::cout << "[MetricsCollector] ⚠ Калибровка уже выполняется" << std::endl;
        return true;  // Уже идет
    }

    // Повторный запуск калибровки (если автокалибровка не сработала или нужно перекалибровать)
    std::cout << "[MetricsCollector] Повторный запуск калибровки..." << std::endl;
    clCError error = clC_Error_OK;
    clCNFBCalibrator_CalibrateIndividualNFBQuick(m_calibrator, &error);

    if (error != clC_Error_OK) {
        std::cerr << "[MetricsCollector] ✗ Ошибка запуска калибровки (error: "
                  << error << ")" << std::endl;
        return false;
    }

    m_calibrating = true;
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
            // Записываем summary перед закрытием предыдущего файла
            writeSummaryToLog();
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

            // Сохраняем время начала для summary
            char datetime_buffer[64];
            std::strftime(datetime_buffer, sizeof(datetime_buffer), "%Y-%m-%d %H:%M:%S", &tm_now);
            m_log_start_time = std::string(datetime_buffer);
        } else {
            m_log_file_path = file_path;

            auto now = std::chrono::system_clock::now();
            auto time_t_now = std::chrono::system_clock::to_time_t(now);
            std::tm tm_now;
            localtime_r(&time_t_now, &tm_now);
            char datetime_buffer[64];
            std::strftime(datetime_buffer, sizeof(datetime_buffer), "%Y-%m-%d %H:%M:%S", &tm_now);
            m_log_start_time = std::string(datetime_buffer);
        }

        // Инициализация счетчиков для summary
        m_log_sample_count = 0;
        m_accumulated_metrics = MetricsSnapshot();

        m_log_file = fopen(m_log_file_path.c_str(), "w");

        if (m_log_file) {
            // Запись metadata в начало файла
            fprintf(m_log_file, "# Bronnikov Exercise App - Metrics Export\n");
            fprintf(m_log_file, "# Session Start: %s\n", m_log_start_time.c_str());
            fprintf(m_log_file, "# File: %s\n", m_log_file_path.c_str());
            fprintf(m_log_file, "#\n");
            fprintf(m_log_file, "# Column Descriptions:\n");
            fprintf(m_log_file, "# - timestamp: Seconds since session start\n");
            fprintf(m_log_file, "# - alpha_power: Alpha brain wave power (8-12 Hz)\n");
            fprintf(m_log_file, "# - beta_power: Beta brain wave power (13-30 Hz)\n");
            fprintf(m_log_file, "# - theta_power: Theta brain wave power (4-8 Hz)\n");
            fprintf(m_log_file, "# - alpha_beta_ratio: Ratio of alpha to beta power\n");
            fprintf(m_log_file, "# - concentration: Concentration score (0-100)\n");
            fprintf(m_log_file, "# - relaxation: Relaxation score (0-100)\n");
            fprintf(m_log_file, "# - fatigue: Fatigue score (0-100)\n");
            fprintf(m_log_file, "# - gravity: Gravity component score\n");
            fprintf(m_log_file, "# - focus: Focus emotion score (0-100)\n");
            fprintf(m_log_file, "# - chill: Chill/calm emotion score (0-100)\n");
            fprintf(m_log_file, "# - stress: Stress emotion score (0-100)\n");
            fprintf(m_log_file, "# - anger: Anger emotion score (0-100)\n");
            fprintf(m_log_file, "# - self_control: Self-control score (0-100)\n");
            fprintf(m_log_file, "# - heart_rate: Heart rate in BPM\n");
            fprintf(m_log_file, "# - stress_index: Cardiovascular stress index\n");
            fprintf(m_log_file, "# - iaf: Individual Alpha Frequency (Hz)\n");
            fprintf(m_log_file, "#\n");

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
            // Записываем summary перед закрытием
            writeSummaryToLog();
            fclose(m_log_file);
            m_log_file = nullptr;
        }
    }
}

void MetricsCollector::enableResearchMode(bool enabled, const std::string& base_path) {
    m_research_mode_enabled = enabled;

    if (enabled) {
        // Закрываем предыдущий файл если был открыт
        if (m_eeg_raw_file) fclose(m_eeg_raw_file);

        // Создаем имя файла
        if (base_path.empty()) {
            auto now = std::chrono::system_clock::now();
            auto time_t_now = std::chrono::system_clock::to_time_t(now);
            std::tm tm_now;
            localtime_r(&time_t_now, &tm_now);

            char timestamp_buffer[64];
            std::strftime(timestamp_buffer, sizeof(timestamp_buffer), "%Y%m%d_%H%M%S", &tm_now);
            m_research_base_path = "research_" + std::string(timestamp_buffer);
        } else {
            m_research_base_path = base_path;
        }

        std::string eeg_path = m_research_base_path + "_eeg_raw.csv";

        // Открываем файл для записи только EEG
        m_eeg_raw_file = fopen(eeg_path.c_str(), "w");

        // Записываем заголовки
        if (m_eeg_raw_file) {
            fprintf(m_eeg_raw_file, "# EEG Raw Data Export\n");
            fprintf(m_eeg_raw_file, "# Timestamp: %s\n", m_log_start_time.c_str());
            fprintf(m_eeg_raw_file, "#\n");
            fprintf(m_eeg_raw_file, "# Channels: F7, F3, Fp1, Fp2, F4, F8, T3, C3, Cz, C4, T4, T5, P3, Pz, P4, T6, O1, O2\n");
            fprintf(m_eeg_raw_file, "#\n");
            fprintf(m_eeg_raw_file, "timestamp,F7,F3,Fp1,Fp2,F4,F8,T3,C3,Cz,C4,T4,T5,P3,Pz,P4,T6,O1,O2\n");
            fflush(m_eeg_raw_file);
        }

        // Устанавливаем callback для raw EEG данных
        clCSessionDelegateSessionEEGData eeg_delegate = clCSession_GetOnSessionEEGDataEvent(m_session);
        clCSessionDelegateSessionEEGData_Set(eeg_delegate, onEEGDataCallback);

        std::cout << "[MetricsCollector] ✓ Research Mode включен (raw EEG)" << std::endl;
        std::cout << "  EEG: " << eeg_path << std::endl;
        std::cout << "  Примечание: PPG и MEMS данные доступны через CCardio и CMEMS классификаторы" << std::endl;
    } else {
        // Закрываем файл
        if (m_eeg_raw_file) {
            fclose(m_eeg_raw_file);
            m_eeg_raw_file = nullptr;
        }

        std::cout << "[MetricsCollector] Research Mode выключен" << std::endl;
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
        std::cout << "[DEBUG]   m_current_metrics updated: concentration="
                  << m_current_metrics.concentration << std::endl;
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

    // DEBUG: Детальное логирование КАЖДОГО обновления эмоций
    static int emotions_update_count = 0;
    emotions_update_count++;

    std::cout << "[MetricsCollector] updateEmotionMetrics #" << emotions_update_count
              << " | Focus=" << states->focus
              << " | Chill=" << states->chill
              << " | Stress=" << states->stress
              << " | Anger=" << states->anger
              << " | SelfControl=" << states->selfControl << std::endl;

    // Предупреждение если все значения = 0
    if (states->focus == 0.0 && states->chill == 0.0 && states->stress == 0.0
        && states->anger == 0.0 && states->selfControl == 0.0) {
        std::cout << "[MetricsCollector] ⚠️ WARNING: Все эмоции = 0! Возможно калибровка еще не завершена." << std::endl;
    }
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

    // Аккумулируем значения для summary
    m_log_sample_count++;
    m_accumulated_metrics.alpha_power += metrics.alpha_power;
    m_accumulated_metrics.beta_power += metrics.beta_power;
    m_accumulated_metrics.theta_power += metrics.theta_power;
    m_accumulated_metrics.alpha_beta_ratio += metrics.alpha_beta_ratio;
    m_accumulated_metrics.concentration += metrics.concentration;
    m_accumulated_metrics.relaxation += metrics.relaxation;
    m_accumulated_metrics.fatigue += metrics.fatigue;
    m_accumulated_metrics.gravity += metrics.gravity;
    m_accumulated_metrics.focus += metrics.focus;
    m_accumulated_metrics.chill += metrics.chill;
    m_accumulated_metrics.stress += metrics.stress;
    m_accumulated_metrics.anger += metrics.anger;
    m_accumulated_metrics.self_control += metrics.self_control;
    m_accumulated_metrics.heart_rate += metrics.heart_rate;
    m_accumulated_metrics.stress_index += metrics.stress_index;
    m_accumulated_metrics.iaf += metrics.iaf;
}

void MetricsCollector::writeSummaryToLog() {
    if (!m_log_file || m_log_sample_count == 0) return;

    // Вычисляем средние значения
    auto calc_average = [this](double accumulated) -> double {
        return accumulated / m_log_sample_count;
    };

    // Получаем текущее время для расчета длительности
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    localtime_r(&time_t_now, &tm_now);
    char end_time_buffer[64];
    std::strftime(end_time_buffer, sizeof(end_time_buffer), "%Y-%m-%d %H:%M:%S", &tm_now);

    // Записываем summary
    fprintf(m_log_file, "\n");
    fprintf(m_log_file, "# ==================== SESSION SUMMARY ====================\n");
    fprintf(m_log_file, "# Session Start: %s\n", m_log_start_time.c_str());
    fprintf(m_log_file, "# Session End: %s\n", end_time_buffer);
    fprintf(m_log_file, "# Total Samples: %d\n", m_log_sample_count);
    fprintf(m_log_file, "#\n");
    fprintf(m_log_file, "# Average Values:\n");
    fprintf(m_log_file, "# - Alpha Power: %.3f\n", calc_average(m_accumulated_metrics.alpha_power));
    fprintf(m_log_file, "# - Beta Power: %.3f\n", calc_average(m_accumulated_metrics.beta_power));
    fprintf(m_log_file, "# - Theta Power: %.3f\n", calc_average(m_accumulated_metrics.theta_power));
    fprintf(m_log_file, "# - Alpha/Beta Ratio: %.3f\n", calc_average(m_accumulated_metrics.alpha_beta_ratio));
    fprintf(m_log_file, "# - Concentration: %.3f\n", calc_average(m_accumulated_metrics.concentration));
    fprintf(m_log_file, "# - Relaxation: %.3f\n", calc_average(m_accumulated_metrics.relaxation));
    fprintf(m_log_file, "# - Fatigue: %.3f\n", calc_average(m_accumulated_metrics.fatigue));
    fprintf(m_log_file, "# - Gravity: %.3f\n", calc_average(m_accumulated_metrics.gravity));
    fprintf(m_log_file, "# - Focus: %.3f\n", calc_average(m_accumulated_metrics.focus));
    fprintf(m_log_file, "# - Chill: %.3f\n", calc_average(m_accumulated_metrics.chill));
    fprintf(m_log_file, "# - Stress: %.3f\n", calc_average(m_accumulated_metrics.stress));
    fprintf(m_log_file, "# - Anger: %.3f\n", calc_average(m_accumulated_metrics.anger));
    fprintf(m_log_file, "# - Self-Control: %.3f\n", calc_average(m_accumulated_metrics.self_control));
    fprintf(m_log_file, "# - Heart Rate: %.3f BPM\n", calc_average(m_accumulated_metrics.heart_rate));
    fprintf(m_log_file, "# - Stress Index: %.3f\n", calc_average(m_accumulated_metrics.stress_index));
    fprintf(m_log_file, "# - IAF: %.3f Hz\n", calc_average(m_accumulated_metrics.iaf));
    fprintf(m_log_file, "# ==========================================================\n");

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

            // ПРИМЕЧАНИЕ: Калибровка NFB опциональна
            // Приложение может работать и без неё, используя стандартные значения IAF
            // Событие OnReadyToCalibrate не вызывается в текущей версии CapsuleAPI
            // Поэтому помечаем калибратор как готовый для ручного запуска
            collector->m_calibrator_ready = true;
            std::cout << "[MetricsCollector] ℹ️  Калибратор готов для ручного запуска" << std::endl;
            std::cout << "[MetricsCollector]    (Калибровка опциональна, приложение работает и без неё)" << std::endl;
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

// Research Mode callbacks для raw данных
void MetricsCollector::onEEGDataCallback(clCSession session, clCEEGTimedData data) {
    auto it = g_session_map.find(session);
    if (it == g_session_map.end() || !it->second || !it->second->m_eeg_raw_file || !data) {
        return;
    }

    MetricsCollector* collector = it->second;

    // EEG данные - это матрица: channels x samples
    int32_t channel_count = clCEEGTimedData_GetChannelsCount(data);
    int32_t sample_count = clCEEGTimedData_GetSamplesCount(data);

    // Экспортируем каждый сэмпл (по всем каналам)
    for (int32_t sample_idx = 0; sample_idx < sample_count; ++sample_idx) {
        // Timestamp в микросекундах, конвертируем в секунды
        uint64_t timestamp_us = clCEEGTimedData_GetTimepoint(data, sample_idx);
        double timestamp_sec = timestamp_us / 1000000.0;
        fprintf(collector->m_eeg_raw_file, "%.6f", timestamp_sec);

        // Записываем значения всех каналов для этого сэмпла
        for (int32_t ch_idx = 0; ch_idx < channel_count && ch_idx < 18; ++ch_idx) {
            float value = clCEEGTimedData_GetValue(data, ch_idx, sample_idx);
            fprintf(collector->m_eeg_raw_file, ",%.6f", value);
        }

        // Заполняем недостающие каналы нулями (если меньше 18)
        for (int32_t ch_idx = channel_count; ch_idx < 18; ++ch_idx) {
            fprintf(collector->m_eeg_raw_file, ",0.0");
        }

        fprintf(collector->m_eeg_raw_file, "\n");
    }

    fflush(collector->m_eeg_raw_file);
}

} // namespace Bronnikov
