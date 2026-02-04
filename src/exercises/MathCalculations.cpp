#include "MathCalculations.h"

namespace Bronnikov {

MathCalculations::MathCalculations()
    : Exercise("Математические вычисления",
               "Использование биокомпьютера для быстрых вычислений и решения задач.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 7;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void MathCalculations::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Задайте простой пример: 23 + 47.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Биокомпьютер показывает ответ на экране.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Усложните: 156 × 7. Получите ответ.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Задайте задачу со словами. Биокомпьютер решает.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Проверьте несколько ответов физически.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void MathCalculations::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +50.0;
    m_target_metrics.concentration_change = +55.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 75.0;
    m_target_metrics.concentration_min = 82.0;
}

} // namespace Bronnikov
