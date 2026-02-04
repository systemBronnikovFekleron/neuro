#include "DistantHealing.h"

namespace Bronnikov {

DistantHealing::DistantHealing()
    : Exercise("Дистанционное целительство",
               "Передача целительной энергии на расстояние через радарное видение.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 21;
    m_duration_minutes = 20;
    initializeInstructions();
    initializeTargetMetrics();
}

void DistantHealing::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Настройтесь на человека (можно фото).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Установите энергетическую связь.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Просканируйте состояние человека радарно.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Направьте целительную энергию в зону проблемы.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте процесс исцеления.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Завершите сеанс, разорвите связь мягко.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void DistantHealing::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +75.0;
    m_target_metrics.theta_change = +58.0;
    m_target_metrics.concentration_change = +70.0;
    m_target_metrics.relaxation_change = +35.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 95.0;
    m_target_metrics.concentration_min = 92.0;
}

} // namespace Bronnikov
