#include "SpatialMemory.h"

namespace Bronnikov {

SpatialMemory::SpatialMemory()
    : Exercise("Пространственное воображение",
               "Развитие объемной памяти и способности удерживать сложные трехмерные образы.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 9;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void SpatialMemory::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Представьте простой куб.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Мысленно поверните куб. Рассмотрите все грани.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте внутрь куба цветной шар.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Усложните: куб в кубе, разные цвета.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте комнату с мебелью. Обойдите мысленно.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК, расслабьтесь.", 30, ExercisePhase::Post});
}

void SpatialMemory::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +40.0;
    m_target_metrics.theta_change = +25.0;
    m_target_metrics.concentration_change = +35.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.7;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 65.0;
}

} // namespace Bronnikov
