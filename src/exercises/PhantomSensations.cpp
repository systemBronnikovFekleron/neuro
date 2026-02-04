#include "PhantomSensations.h"

namespace Bronnikov {

PhantomSensations::PhantomSensations()
    : Exercise("Развитие фантомных чувств",
               "Упражнения 'мяч', 'гармошка', 'луч' для активизации биоэнергетической чувствительности.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 2;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void PhantomSensations::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Потрите ладони до тепла.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"'Гармошка': сжимайте и растягивайте энергию между ладонями.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"'Мяч': почувствуйте упругий энергетический шар между ладонями.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Вращайте мяч, меняя положение рук.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"'Луч': указательным пальцем рисуйте фигуры на ладони другой руки.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выполняйте упражнения на разных расстояниях, сравнивайте ощущения.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void PhantomSensations::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +38.0;
    m_target_metrics.theta_change = +22.0;
    m_target_metrics.concentration_change = +48.0;
    m_target_metrics.relaxation_change = +35.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.0;
    m_target_metrics.alpha_min = 52.0;
    m_target_metrics.theta_min = 27.0;
    m_target_metrics.concentration_min = 58.0;
}

} // namespace Bronnikov
