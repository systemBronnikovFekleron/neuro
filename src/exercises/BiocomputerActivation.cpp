#include "BiocomputerActivation.h"

namespace Bronnikov {

BiocomputerActivation::BiocomputerActivation()
    : Exercise("Активация биокомпьютера",
               "Запуск и настройка личного биокомпьютера (ЛБК) для работы с информацией.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 1;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void BiocomputerActivation::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Создайте белый экран перед собой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Активируйте центральную точку управления биокомпьютером.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Настройте яркость и чёткость экрана до максимума.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Проверьте связь: задайте простой вопрос, получите ответ.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Зафиксируйте стабильное состояние биокомпьютера.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК постепенно.", 30, ExercisePhase::Post});
}

void BiocomputerActivation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +50.0;
    m_target_metrics.concentration_change = +45.0;
    m_target_metrics.theta_change = +25.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 75.0;
    m_target_metrics.concentration_min = 78.0;
}

} // namespace Bronnikov
