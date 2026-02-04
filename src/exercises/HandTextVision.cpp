#include "HandTextVision.h"

namespace Bronnikov {

HandTextVision::HandTextVision()
    : Exercise("Видеть рукой письменные знаки",
               "Распознавание букв, цифр и слов через тактильный биоэнергетический контакт.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 13;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void HandTextVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Положите руку на карточку с буквой.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите форму буквы через биоконтакт.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите букву, видимую на экране ЛБК.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте распознать цифры 0-9.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Перейдите к простым словам (2-3 буквы).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void HandTextVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +45.0;
    m_target_metrics.concentration_change = +42.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 70.0;
    m_target_metrics.concentration_min = 75.0;
    m_target_metrics.focus_min = 72.0;
}

} // namespace Bronnikov
