#include "NumberTable.h"

namespace Bronnikov {

NumberTable::NumberTable()
    : Exercise("Таблица с числами",
               "Работа с числовой информацией на экране ЛБК. Развитие памяти и визуализации.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 6;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void NumberTable::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка на глазах. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Белый экран.", 30, ExercisePhase::Execution});
    m_instructions.push_back({"Нарисуйте черными линиями квадратную рамку (клетку).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"В центре рамки нарисуйте черную цифру 1.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Проколите цифру иголочкой, поверните как стрелку часов в горизонтальное положение.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Нарисуйте таблицу 2x2 (четыре клетки). Заполните числами 1, 2, 3, 4.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите все числа и их места в таблице. Уберите таблицу. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void NumberTable::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +25.0;
    m_target_metrics.concentration_change = +30.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 50.0;
    m_target_metrics.concentration_min = 60.0;
}

} // namespace Bronnikov
