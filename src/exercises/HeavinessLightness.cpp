#include "HeavinessLightness.h"

namespace Bronnikov {

HeavinessLightness::HeavinessLightness()
    : Exercise("Тяжесть и легкость",
               "Аутотренинг формирования ощущений тяжести и легкости. Система взаимодействия сил Неба и Земли.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 4;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void HeavinessLightness::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Спокойное дыхание.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Команда 'Тяжесть': представьте себя пустым сосудом.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Ориентируйтесь на нисходящий поток солнечной энергии.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Почувствуйте, как тело наполняется, становится тяжелым.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Команда 'Легкость': ориентируйтесь на восходящий космический поток.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Почувствуйте, как тело теряет вес, увеличивается в объеме, тянет вверх.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Чередуйте состояния: отдельные участки тела тяжелые/легкие.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void HeavinessLightness::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +40.0;
    m_target_metrics.theta_change = +28.0;
    m_target_metrics.concentration_change = +50.0;
    m_target_metrics.relaxation_change = +45.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.2;
    m_target_metrics.alpha_min = 53.0;
    m_target_metrics.theta_min = 32.0;
    m_target_metrics.concentration_min = 60.0;
}

} // namespace Bronnikov
