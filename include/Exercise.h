#pragma once

#include "MetricsCollector.h"
#include "SessionDatabase.h"
#include <string>
#include <vector>
#include <functional>

namespace Bronnikov {

/**
 * @brief Ступени обучения по методике Бронникова
 */
enum class ExerciseStage {
    Preparatory = 0,  // Подготовительная ступень (базовая энергетика - 10 упражнений)
    Stage1 = 1,       // 1-я ступень: Экология духа (18 упражнений из методички 2011)
    Stage2 = 2,       // 2-я ступень: Внутреннее видение (25 упражнений альтернативного видения)
    Stage3 = 3,       // 3-я ступень: Радарное видение (25 упражнений информационного развития)
    Stage4 = 4        // 4-я ступень: НЕ ИСПОЛЬЗУЕТСЯ (упражнения перенесены в Stage3)
};

/**
 * @brief Получить название ступени на русском
 */
inline std::string getStageDisplayName(ExerciseStage stage) {
    switch (stage) {
        case ExerciseStage::Preparatory: return "Подготовительная ступень";
        case ExerciseStage::Stage1:      return "1-я ступень: Экология духа";
        case ExerciseStage::Stage2:      return "2-я ступень: Внутреннее видение";
        case ExerciseStage::Stage3:      return "3-я ступень: Радарное видение";
        case ExerciseStage::Stage4:      return "4-я ступень: НЕ ИСПОЛЬЗУЕТСЯ";
        default:                         return "Неизвестная ступень";
    }
}

/**
 * @brief Получить краткое название ступени
 */
inline std::string getStageShortName(ExerciseStage stage) {
    switch (stage) {
        case ExerciseStage::Preparatory: return "Подготовительная";
        case ExerciseStage::Stage1:      return "1-я ступень";
        case ExerciseStage::Stage2:      return "2-я ступень";
        case ExerciseStage::Stage3:      return "3-я ступень";
        case ExerciseStage::Stage4:      return "4-я ступень";
        default:                         return "???";
    }
}

/**
 * @brief Фазы выполнения упражнения
 */
enum class ExercisePhase {
    NotStarted,     // Упражнение еще не начато
    Preparation,    // Подготовительная фаза
    Baseline,       // НОВОЕ: Запись базовых метрик (до упражнения)
    Execution,      // Основная фаза выполнения
    Post,           // НОВОЕ: Запись финальных метрик (после упражнения)
    Completion,     // Завершающая фаза
    Finished        // Упражнение завершено
};

/**
 * @brief Целевые метрики для упражнения
 */
struct TargetMetrics {
    // Диапазоны для основных метрик (min, max)
    double alpha_min = 0.0, alpha_max = 100.0;
    double beta_min = 0.0, beta_max = 100.0;
    double theta_min = 0.0, theta_max = 100.0;

    double concentration_min = 0.0, concentration_max = 100.0;
    double relaxation_min = 0.0, relaxation_max = 100.0;
    double focus_min = 0.0, focus_max = 100.0;

    // Ожидаемые изменения относительно baseline (в %)
    double alpha_change = 0.0;        // Например, +15% означает увеличение на 15%
    double beta_change = 0.0;
    double theta_change = 0.0;
    double concentration_change = 0.0;
    double relaxation_change = 0.0;

    // Важность метрик (0-1, где 1 - наиболее важная)
    double alpha_weight = 1.0;
    double beta_weight = 0.5;
    double theta_weight = 0.5;
    double concentration_weight = 1.0;
    double relaxation_weight = 1.0;
};

/**
 * @brief Инструкция для выполнения упражнения
 */
struct Instruction {
    std::string text;           // Текст инструкции
    int duration_seconds = 0;   // Длительность выполнения (0 = без ограничения)
    ExercisePhase phase = ExercisePhase::Execution;
};

/**
 * @brief НОВОЕ: Метрики трех фаз упражнения
 */
struct PhaseMetrics {
    MetricsSnapshot baseline;   // Базовые метрики (до упражнения)
    MetricsSnapshot active;     // Метрики во время упражнения (усредненные)
    MetricsSnapshot post;       // Финальные метрики (после упражнения)

    int baseline_samples = 0;   // Количество измерений в baseline фазе
    int active_samples = 0;     // Количество измерений в active фазе
    int post_samples = 0;       // Количество измерений в post фазе

    bool has_baseline = false;  // Собраны ли baseline метрики
    bool has_active = false;    // Собраны ли active метрики
    bool has_post = false;      // Собраны ли post метрики
};

/**
 * @brief НОВОЕ: Результат сравнения фаз
 */
struct ComparisonResult {
    // Изменения Alpha (активность)
    double alpha_change = 0.0;          // % изменения (post - baseline)
    double alpha_active_change = 0.0;   // % изменения (active - baseline)

    // Изменения Beta (концентрация)
    double beta_change = 0.0;
    double beta_active_change = 0.0;

    // Изменения Theta (релаксация)
    double theta_change = 0.0;
    double theta_active_change = 0.0;

    // Изменения продуктивности
    double concentration_change = 0.0;
    double concentration_active_change = 0.0;

    double relaxation_change = 0.0;
    double relaxation_active_change = 0.0;

    double fatigue_change = 0.0;
    double fatigue_active_change = 0.0;

    // Общая оценка эффективности упражнения
    double overall_effectiveness = 0.0;  // 0-100%

    // Достигнуты ли целевые изменения
    bool target_achieved = false;
};

/**
 * @brief Базовый класс для упражнений методики Бронникова
 *
 * Каждое упражнение наследуется от этого класса и определяет
 * свои целевые метрики, инструкции и критерии успеха.
 */
class Exercise {
public:
    Exercise(const std::string& name, const std::string& description);
    virtual ~Exercise() = default;

    /**
     * @brief Запуск упражнения
     * @param metrics_collector Коллектор метрик для мониторинга
     */
    virtual void start(MetricsCollector& metrics_collector);

    /**
     * @brief Обновление состояния упражнения
     * @param current_metrics Текущие метрики мозга
     * @param elapsed_seconds Время с начала упражнения
     */
    virtual void update(const MetricsSnapshot& current_metrics, double elapsed_seconds);

    /**
     * @brief Завершение упражнения
     */
    virtual void finish();

    /**
     * @brief НОВОЕ: Начать запись baseline метрик (до упражнения)
     * Собирает метрики в течение 30-60 секунд в состоянии покоя
     */
    virtual void startBaseline();

    /**
     * @brief НОВОЕ: Начать основную фазу практики
     * Переход от baseline к активному выполнению
     */
    virtual void startActive();

    /**
     * @brief НОВОЕ: Начать запись post метрик (после упражнения)
     * Собирает метрики в течение 30-60 секунд после завершения
     */
    virtual void startPost();

    /**
     * @brief НОВОЕ: Сравнить метрики трех фаз
     * @return Результат сравнения с процентными изменениями
     */
    virtual ComparisonResult comparePhases() const;

    /**
     * @brief Проверка достижения целевого состояния
     * @param metrics Текущие метрики
     * @return Оценка соответствия целевому состоянию (0-100%)
     */
    virtual double checkTargetState(const MetricsSnapshot& metrics) const;

    /**
     * @brief Проверка успешного завершения упражнения
     * @return Оценка успешности (0-100%)
     */
    virtual double calculateSuccessRate() const;

    // Геттеры
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }
    int getDuration() const { return m_duration_minutes; }
    ExercisePhase getCurrentPhase() const { return m_current_phase; }
    const std::vector<Instruction>& getInstructions() const { return m_instructions; }
    const TargetMetrics& getTargetMetrics() const { return m_target_metrics; }

    // НОВЫЕ геттеры для поддержки ступеней
    ExerciseStage getStage() const { return m_stage; }
    int getOrderInStage() const { return m_order_in_stage; }
    std::string getStageDisplayName() const {
        return Bronnikov::getStageDisplayName(m_stage);
    }

    // Уникальный идентификатор упражнения: "0.1", "1.5", "2.3"
    std::string getStageId() const {
        return std::to_string(static_cast<int>(m_stage)) + "." +
               std::to_string(m_order_in_stage);
    }

    /**
     * @brief Получение текущей инструкции
     */
    const Instruction* getCurrentInstruction() const;

    /**
     * @brief Переход к следующей фазе
     */
    void nextPhase();

    /**
     * @brief Получение статистики выполнения
     */
    struct Statistics {
        double total_duration = 0.0;           // Общее время выполнения
        double time_in_target_state = 0.0;     // Время в целевом состоянии
        double average_target_score = 0.0;     // Средняя оценка соответствия
        double max_target_score = 0.0;         // Максимальная оценка
        double success_rate = 0.0;             // Итоговая оценка успеха
    };
    const Statistics& getStatistics() const { return m_statistics; }

    /**
     * @brief НОВОЕ: Получение метрик трех фаз
     */
    const PhaseMetrics& getPhaseMetrics() const { return m_phase_metrics; }

    // Callbacks
    using PhaseCallback = std::function<void(ExercisePhase phase)>;
    using InstructionCallback = std::function<void(const Instruction& instruction)>;
    using ProgressCallback = std::function<void(double progress_percent, double target_score)>;

    void setOnPhaseChangedCallback(PhaseCallback callback) { m_on_phase_changed = callback; }
    void setOnNewInstructionCallback(InstructionCallback callback) { m_on_new_instruction = callback; }
    void setOnProgressUpdateCallback(ProgressCallback callback) { m_on_progress_update = callback; }

    /**
     * @brief Установить длительность упражнения вручную
     * @param minutes Длительность в минутах
     */
    void setDuration(int minutes) {
        m_duration_minutes = minutes;
    }

    /**
     * @brief Установить рекомендуемую длительность на основе уровня практики
     * @param level Уровень практики пользователя
     * @param use_max Использовать максимум диапазона (true) или минимум (false)
     */
    void setRecommendedDuration(PracticeLevel level, bool use_max = false);

protected:
    std::string m_name;
    std::string m_description;
    int m_duration_minutes = 10;

    // НОВЫЕ ПОЛЯ для поддержки ступеней
    ExerciseStage m_stage = ExerciseStage::Preparatory;  // Ступень обучения
    int m_order_in_stage = 1;                             // Порядковый номер в ступени (1, 2, 3...)

    ExercisePhase m_current_phase = ExercisePhase::NotStarted;
    std::vector<Instruction> m_instructions;
    TargetMetrics m_target_metrics;

    size_t m_current_instruction_index = 0;
    double m_phase_start_time = 0.0;

    Statistics m_statistics;
    std::vector<double> m_target_scores;  // История оценок соответствия

    // НОВОЕ: Трехфазные метрики
    PhaseMetrics m_phase_metrics;

    PhaseCallback m_on_phase_changed;
    InstructionCallback m_on_new_instruction;
    ProgressCallback m_on_progress_update;

    /**
     * @brief Инициализация инструкций (переопределяется в подклассах)
     */
    virtual void initializeInstructions() = 0;

    /**
     * @brief Инициализация целевых метрик (переопределяется в подклассах)
     */
    virtual void initializeTargetMetrics() = 0;

    /**
     * @brief Проверка условия перехода к следующей фазе
     */
    virtual bool shouldMoveToNextPhase(double elapsed_seconds) const;
};

} // namespace Bronnikov
