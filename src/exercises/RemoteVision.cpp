#include "RemoteVision.h"

namespace Bronnikov {

RemoteVision::RemoteVision()
    : Exercise("Дистанционно-проникающее зрение",
               "Видение объектов и событий на расстоянии без физического контакта.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 22;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void RemoteVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Представьте знакомое место (дом, улицу).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Переместите своё внимание туда через ЛБК.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите детали: что происходит, кто там.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Переместитесь в другое место (по выбору).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Вернитесь в своё тело. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void RemoteVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +58.0;
    m_target_metrics.theta_change = +40.0;
    m_target_metrics.concentration_change = +52.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 82.0;
}

} // namespace Bronnikov
