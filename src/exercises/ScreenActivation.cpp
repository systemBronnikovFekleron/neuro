#include "ScreenActivation.h"

namespace Bronnikov {

ScreenActivation::ScreenActivation()
    : Exercise("Включение экрана внутреннего видения",
               "Активизация ПсихоБиоКомпьютера. Формирование внутреннего объема и яркой точки.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 18;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void ScreenActivation::initializeInstructions() {
    m_instructions.push_back({"ТРЕБОВАНИЯ: Поле подсознания ≥7м, поле сознания ≥60см. Проверка готовности.", 60, ExercisePhase::Baseline});
    m_instructions.push_back({"Сделайте несколько всплесков для активизации.", 60, ExercisePhase::Baseline});
    m_instructions.push_back({"Наденьте маску. Закройте глаза. Проверьте: свет не попадает под маску.", 60, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Нейтральное состояние.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Формирование внутреннего объема: создайте темное пространство перед собой.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"В центре темного пространства создайте яркую точку.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Удерживайте стабильность: темное пространство + яркая точка в центре.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Тренируйте устойчивость экрана: точка не должна исчезать или расплываться.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"ВАЖНО: Руки и ноги не перекрещивать. О негативных ощущениях сообщайте сразу.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Сделайте всплеск. Снимите маску ТОЛЬКО после всплеска.", 60, ExercisePhase::Post});
    m_instructions.push_back({"Выключите ЛБК очень медленно.", 30, ExercisePhase::Post});
}

void ScreenActivation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +55.0;
    m_target_metrics.theta_change = +38.0;
    m_target_metrics.concentration_change = +65.0;
    m_target_metrics.relaxation_change = +30.0;
    m_target_metrics.alpha_weight = 1.3;
    m_target_metrics.theta_weight = 1.1;
    m_target_metrics.concentration_weight = 1.3;
    m_target_metrics.relaxation_weight = 0.7;
    m_target_metrics.alpha_min = 68.0;
    m_target_metrics.theta_min = 43.0;
    m_target_metrics.concentration_min = 75.0;
}

} // namespace Bronnikov
