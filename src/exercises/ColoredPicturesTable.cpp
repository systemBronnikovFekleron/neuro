#include "ColoredPicturesTable.h"

namespace Bronnikov {

ColoredPicturesTable::ColoredPicturesTable()
    : Exercise("Таблица с цветными картинками",
               "Визуализация сложных цветных образов в таблицах на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 8;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void ColoredPicturesTable::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Нарисуйте таблицу 2x2.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Первая клетка: нарисуйте синее яблоко.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Вторая клетка: нарисуйте красную розу.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Третья клетка: нарисуйте зелёный лист.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Четвёртая клетка: нарисуйте жёлтое солнце.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите все образы. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ColoredPicturesTable::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +35.0;
    m_target_metrics.concentration_change = +30.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 60.0;
    m_target_metrics.concentration_min = 65.0;
}

} // namespace Bronnikov
