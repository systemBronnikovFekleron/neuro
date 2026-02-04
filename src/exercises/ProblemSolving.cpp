#include "ProblemSolving.h"

namespace Bronnikov {

ProblemSolving::ProblemSolving()
    : Exercise("Решение сложных задач",
               "Использование биокомпьютера для анализа и решения нетривиальных проблем.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 12;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void ProblemSolving::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Сформулируйте проблему чётко.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Загрузите все известные данные о проблеме.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Запросите у биокомпьютера варианты решений.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите каждый вариант, оцените плюсы и минусы.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Выберите оптимальное решение.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ProblemSolving::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +62.0;
    m_target_metrics.theta_change = +42.0;
    m_target_metrics.concentration_change = +58.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 85.0;
    m_target_metrics.concentration_min = 85.0;
}

} // namespace Bronnikov
