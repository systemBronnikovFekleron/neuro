#include "EnergeticCocoon.h"

namespace Bronnikov {

EnergeticCocoon::EnergeticCocoon()
    : Exercise("Энергетический кокон",
               "Согласование взаимодействия внутренней и внешней энергетических структур.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 11;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void EnergeticCocoon::initializeInstructions() {
    m_instructions.push_back({"Поза стоя. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Руки на уровне груди ладонями к себе, 20-40 см от тела.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Мысленная команда: почувствовать энергетический кокон.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Поглаживающе-прихлопывающие движения вокруг тела.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите упругую плотность под ладонями, покалывание в теле.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ощупайте энергетический скафандр в разных участках.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Убедитесь в однородности ощущений.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Остановите физические руки. 'Энергетические руки' продолжают движение.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Вращайте кокон в разные стороны, сдвигайте вверх-вниз мысленно.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void EnergeticCocoon::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +44.0;
    m_target_metrics.theta_change = +27.0;
    m_target_metrics.concentration_change = +54.0;
    m_target_metrics.relaxation_change = +37.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.0;
    m_target_metrics.alpha_min = 57.0;
    m_target_metrics.theta_min = 32.0;
    m_target_metrics.concentration_min = 64.0;
}

} // namespace Bronnikov
