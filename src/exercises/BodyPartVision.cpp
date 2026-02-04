#include "BodyPartVision.h"

namespace Bronnikov {

BodyPartVision::BodyPartVision()
    : Exercise("Видеть любой частью тела",
               "Развитие способности воспринимать визуальную информацию различными частями тела.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 14;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void BodyPartVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Прикоснитесь локтем к объекту.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Переведите ощущение на экран ЛБК.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте видеть плечом, затем коленом.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Распознайте цвет объекта любой частью тела.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void BodyPartVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +48.0;
    m_target_metrics.concentration_change = +40.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 72.0;
}

} // namespace Bronnikov
