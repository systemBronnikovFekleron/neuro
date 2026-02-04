#include "MultidimensionalPerception.h"

namespace Bronnikov {

MultidimensionalPerception::MultidimensionalPerception()
    : Exercise("Многомерное восприятие",
               "Восприятие реальности в нескольких измерениях одновременно.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 24;
    m_duration_minutes = 24;
    initializeInstructions();
    initializeTargetMetrics();
}

void MultidimensionalPerception::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Войдите в состояние глубокой медитации.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Расширьте восприятие за пределы трёхмерного пространства.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Увидьте объекты в 4-м измерении (время).", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите другие измерения: энергетическое, информационное.", 300, ExercisePhase::Execution});
    m_instructions.push_back({"Удерживайте многомерное восприятие.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК очень медленно.", 30, ExercisePhase::Post});
}

void MultidimensionalPerception::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +82.0;
    m_target_metrics.theta_change = +68.0;
    m_target_metrics.concentration_change = +78.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 98.0;
    m_target_metrics.theta_min = 52.0;
    m_target_metrics.concentration_min = 95.0;
}

} // namespace Bronnikov
