#include "ObjectHistory.h"

namespace Bronnikov {

ObjectHistory::ObjectHistory()
    : Exercise("Считывание истории объектов",
               "Восприятие информационного следа: история владения, события, эмоции.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 20;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void ObjectHistory::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Возьмите в руки предмет (кольцо, книга).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Войдите в контакт с информационным полем объекта.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите энергию предыдущего владельца.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте ключевые события из истории предмета.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Определите эмоции, связанные с объектом.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ObjectHistory::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +66.0;
    m_target_metrics.theta_change = +53.0;
    m_target_metrics.concentration_change = +60.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 88.0;
}

} // namespace Bronnikov
