#include "MorphologicalFields.h"

namespace Bronnikov {

MorphologicalFields::MorphologicalFields()
    : Exercise("Определение параметров морфологических полей",
               "Измерение горизонтального (7+ метров) и вертикального (60+ см) биополя человека.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 1;
    m_duration_minutes = 8;
    initializeInstructions();
    initializeTargetMetrics();
}

void MorphologicalFields::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Спокойное состояние.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель измеряет горизонтальное поле (от ладони до тела).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Норма: 7 метров и более. Поле должно быть плотным, равномерным.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель измеряет вертикальное поле над головой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Норма: 60 см и более над головой.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void MorphologicalFields::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +35.0;
    m_target_metrics.theta_change = +20.0;
    m_target_metrics.concentration_change = +45.0;
    m_target_metrics.relaxation_change = +30.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 0.9;
    m_target_metrics.alpha_min = 50.0;
    m_target_metrics.theta_min = 25.0;
    m_target_metrics.concentration_min = 55.0;
}

} // namespace Bronnikov
