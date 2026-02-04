#include "ObjectVision.h"

namespace Bronnikov {

ObjectVision::ObjectVision()
    : Exercise("Видеть посредством любого предмета",
               "Использование предметов как удлинителей биоэнергетической чувствительности.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 16;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void ObjectVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Возьмите карандаш в руку.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Прикоснитесь карандашом к цветному объекту.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите цвет через карандаш, выведите на ЛБК.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте с другими предметами: ложка, линейка.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ObjectVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +45.0;
    m_target_metrics.concentration_change = +40.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 70.0;
}

} // namespace Bronnikov
