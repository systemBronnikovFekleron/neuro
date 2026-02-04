#include "RouteMemory.h"

namespace Bronnikov {

RouteMemory::RouteMemory()
    : Exercise("Путевая (маршрутная) память",
               "Запоминание и воспроизведение маршрутов в пространстве на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 10;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void RouteMemory::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Вспомните знакомый маршрут.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте начальную точку маршрута.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Мысленно пройдите по маршруту, запоминая детали.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Воспроизведите маршрут в обратном направлении.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите ключевые точки маршрута.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void RouteMemory::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +35.0;
    m_target_metrics.theta_change = +20.0;
    m_target_metrics.concentration_change = +30.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 60.0;
}

} // namespace Bronnikov
