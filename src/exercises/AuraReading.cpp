#include "AuraReading.h"

namespace Bronnikov {

AuraReading::AuraReading()
    : Exercise("Чтение ауры",
               "Восприятие и интерпретация энергетической ауры живых существ.")
{
    m_stage = ExerciseStage::Stage4;
    m_order_in_stage = 2;
    m_duration_minutes = 16;
    initializeInstructions();
    initializeTargetMetrics();
}

void AuraReading::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активируйте режим чтения ауры.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Направьте внимание на человека (можно себя).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте ауру: цвета, слои, свечение.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Определите эмоциональное состояние по цвету.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"Найдите зоны напряжения или дисбаланса.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void AuraReading::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +68.0;
    m_target_metrics.theta_change = +50.0;
    m_target_metrics.concentration_change = +62.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 90.0;
}

} // namespace Bronnikov
