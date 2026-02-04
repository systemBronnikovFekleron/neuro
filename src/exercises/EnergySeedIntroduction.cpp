#include "EnergySeedIntroduction.h"

namespace Bronnikov {

EnergySeedIntroduction::EnergySeedIntroduction()
    : Exercise("Введение энергетических сущностей",
               "Техника введения энергетических сущностей в тело человека и их перемещение.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 5;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void EnergySeedIntroduction::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Учитель активизирует энергию над правой ладонью ученика.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель перемещает энергию вращательными движениями вверх по руке.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Энергия идет через плечи, воротниковую зону, вниз по левой руке.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик фиксирует ощущения каждые 5-10 см. При ослаблении - сообщает.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Повторить круг 2-3 раза с помощью учителя.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик перемещает энергию самостоятельно по кругу.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ускоряйте, замедляйте, останавливайте поток в любом месте.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void EnergySeedIntroduction::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +43.0;
    m_target_metrics.theta_change = +26.0;
    m_target_metrics.concentration_change = +54.0;
    m_target_metrics.relaxation_change = +36.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.1;
    m_target_metrics.relaxation_weight = 0.9;
    m_target_metrics.alpha_min = 56.0;
    m_target_metrics.theta_min = 31.0;
    m_target_metrics.concentration_min = 64.0;
}

} // namespace Bronnikov
