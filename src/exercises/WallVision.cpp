#include "WallVision.h"

namespace Bronnikov {

WallVision::WallVision()
    : Exercise("Видеть сквозь стену",
               "Проникающее видение сквозь физические барьеры на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 24;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void WallVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Встаньте перед стеной.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Направьте ЛБК на стену, проникните сквозь неё.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Опишите, что видите за стеной: цвета, формы, объекты.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте увидеть конкретный предмет (выбранный заранее).", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Проверьте результат физически. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void WallVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +65.0;
    m_target_metrics.theta_change = +45.0;
    m_target_metrics.concentration_change = +60.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 88.0;
}

} // namespace Bronnikov
