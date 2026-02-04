#include "HandColorVision.h"

namespace Bronnikov {

HandColorVision::HandColorVision()
    : Exercise("Видеть рукой цвет",
               "Развитие способности распознавать цвет объектов через тактильный контакт.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 11;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void HandColorVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Положите руку на цветную карточку.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Почувствуйте энергию цвета через ладонь.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите цвет, который видите на экране ЛБК.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Смените карточку. Повторите распознавание.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте с 3-4 разными цветами.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void HandColorVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +40.0;
    m_target_metrics.concentration_change = +35.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 65.0;
    m_target_metrics.concentration_min = 70.0;
    m_target_metrics.focus_min = 65.0;
}

} // namespace Bronnikov
