#include "TimeManagement.h"

namespace Bronnikov {

TimeManagement::TimeManagement()
    : Exercise("Управление временем через биокомпьютер",
               "Планирование задач, напоминания и управление расписанием через ЛБК.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 9;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void TimeManagement::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Создайте календарь на экране.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте задачу на завтра с описанием.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Установите напоминание на определённое время.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Просмотрите расписание на неделю.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Измените время одной из задач.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void TimeManagement::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +48.0;
    m_target_metrics.concentration_change = +46.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 72.0;
    m_target_metrics.concentration_min = 78.0;
}

} // namespace Bronnikov
