#include "OrganDiagnostics.h"

namespace Bronnikov {

OrganDiagnostics::OrganDiagnostics()
    : Exercise("Диагностика внутренних органов",
               "Глубокое сканирование состояния внутренних органов через радарное видение.")
{
    m_stage = ExerciseStage::Stage4;
    m_order_in_stage = 4;
    m_duration_minutes = 22;
    initializeInstructions();
    initializeTargetMetrics();
}

void OrganDiagnostics::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активируйте режим глубокого сканирования.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Просканируйте сердце: форма, ритм, энергетика.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Перейдите к печени. Оцените состояние, плотность.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Исследуйте почки, желудок, поджелудочную железу.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Найдите зоны воспаления или дисфункции.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Составьте общую картину состояния организма.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void OrganDiagnostics::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +72.0;
    m_target_metrics.theta_change = +55.0;
    m_target_metrics.concentration_change = +68.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 95.0;
    m_target_metrics.concentration_min = 92.0;
}

} // namespace Bronnikov
