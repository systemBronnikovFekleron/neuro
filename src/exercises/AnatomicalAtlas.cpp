#include "AnatomicalAtlas.h"

namespace Bronnikov {

AnatomicalAtlas::AnatomicalAtlas()
    : Exercise("Работа через Анатомический атлас",
               "Оздоровление и развитие органов и систем организма через схемы Анатомического атласа.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 16;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void AnatomicalAtlas::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Анатомический атлас перед вами. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Человек находится рядом (или представьте его образ).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте рукой энергетический контакт (связь) с человеком.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Перенесите эту связь на рисунок нужного органа в атласе.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Работайте с фантомными ощущениями по рисунку в атласе.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Вращательные движения ладонью активизируют орган через схему.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Работа идет 3-5 минут на один орган/систему.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"По окончании работы разорвите энергетический контакт.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void AnatomicalAtlas::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +52.0;
    m_target_metrics.theta_change = +35.0;
    m_target_metrics.concentration_change = +62.0;
    m_target_metrics.relaxation_change = +36.0;
    m_target_metrics.alpha_weight = 1.1;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.2;
    m_target_metrics.relaxation_weight = 0.9;
    m_target_metrics.alpha_min = 64.0;
    m_target_metrics.theta_min = 40.0;
    m_target_metrics.concentration_min = 72.0;
}

} // namespace Bronnikov
