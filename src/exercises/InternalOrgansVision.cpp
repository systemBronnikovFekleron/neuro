#include "InternalOrgansVision.h"

namespace Bronnikov {

InternalOrgansVision::InternalOrgansVision()
    : Exercise("Видеть свои внутренние органы",
               "Визуализация собственных внутренних органов на экране ЛБК для диагностики.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 15;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void InternalOrgansVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Направьте внимание в область сердца.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте форму и цвет сердца на ЛБК.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Переместите внимание на печень. Рассмотрите.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Просканируйте желудок, почки, лёгкие.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Отметьте зоны дискомфорта или изменений.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void InternalOrgansVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +50.0;
    m_target_metrics.theta_change = +30.0;
    m_target_metrics.concentration_change = +45.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 75.0;
}

} // namespace Bronnikov
