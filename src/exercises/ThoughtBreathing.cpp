#include "ThoughtBreathing.h"

namespace Bronnikov {

ThoughtBreathing::ThoughtBreathing()
    : Exercise("Дыхание сущностью мысли",
               "Дыхание сущностью мысли через части тела. Дистанционная передача энергии.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 14;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void ThoughtBreathing::initializeInstructions() {
    m_instructions.push_back({"Поза стоя лицом друг к другу на расстоянии 50 см. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Руки на уровне плеч ладонями вперед, согнуты в локтях.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Потрите ладони, плавно раскачивая вперед-назад, законтактируйте.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик отходит на расстояние 5-7 метров, сохраняя энергетический контакт.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик работает как 'приемник' ощущений.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель: плавные движения руками вперед-назад на уровне груди.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель с выдохом посылает энергопоток из ладоней в ладони ученика.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик с вдохом принимает отраженный сигнал.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Попеременно: правая, левая, обе руки. Затем смена ролей.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void ThoughtBreathing::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +46.0;
    m_target_metrics.theta_change = +29.0;
    m_target_metrics.concentration_change = +56.0;
    m_target_metrics.relaxation_change = +34.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.1;
    m_target_metrics.relaxation_weight = 0.9;
    m_target_metrics.alpha_min = 59.0;
    m_target_metrics.theta_min = 34.0;
    m_target_metrics.concentration_min = 66.0;
}

} // namespace Bronnikov
