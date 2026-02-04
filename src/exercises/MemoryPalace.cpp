#include "MemoryPalace.h"

namespace Bronnikov {

MemoryPalace::MemoryPalace()
    : Exercise("Дворец памяти в биокомпьютере",
               "Создание многоуровневой структуры для хранения информации в ЛБК.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 5;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void MemoryPalace::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Представьте дворец с комнатами.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте первую комнату - 'Работа'.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Разместите в ней информацию (документы, образы).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте вторую комнату - 'Учёба'. Заполните.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Пройдитесь по комнатам, проверьте доступ к информации.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void MemoryPalace::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +58.0;
    m_target_metrics.theta_change = +35.0;
    m_target_metrics.concentration_change = +52.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 80.0;
}

} // namespace Bronnikov
