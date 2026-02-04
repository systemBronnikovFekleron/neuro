#include "ColoredCellsTable.h"

namespace Bronnikov {

ColoredCellsTable::ColoredCellsTable()
    : Exercise("Таблица с цветными клетками",
               "Работа с цветовой информацией в таблицах на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 7;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void ColoredCellsTable::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Нарисуйте таблицу 2x2 (четыре квадрата).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Заполните первую клетку синим цветом. Яркая заливка.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Заполните вторую клетку красным цветом.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Заполните третью клетку зелёным цветом.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Заполните четвёртую клетку жёлтым цветом.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите цвета всех клеток. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ColoredCellsTable::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +30.0;
    m_target_metrics.concentration_change = +25.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 55.0;
    m_target_metrics.concentration_min = 60.0;
}

} // namespace Bronnikov
