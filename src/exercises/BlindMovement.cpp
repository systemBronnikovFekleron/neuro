#include "BlindMovement.h"

namespace Bronnikov {

BlindMovement::BlindMovement()
    : Exercise("Перемещение с повязкой на глазах",
               "Практическое применение альтернативного зрения для навигации в пространстве.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 21;
    m_duration_minutes = 20;
    initializeInstructions();
    initializeTargetMetrics();
}

void BlindMovement::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Встаньте в знакомом помещении.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Медленно пройдите вперёд, видя через ЛБК.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Обойдите препятствие (стул, стол).", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Найдите и возьмите заранее положенный предмет.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Вернитесь в исходную точку.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Сядьте. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void BlindMovement::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +60.0;
    m_target_metrics.concentration_change = +55.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 80.0;
    m_target_metrics.concentration_min = 82.0;
}

} // namespace Bronnikov
