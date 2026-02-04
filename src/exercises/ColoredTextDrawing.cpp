#include "ColoredTextDrawing.h"

namespace Bronnikov {

ColoredTextDrawing::ColoredTextDrawing()
    : Exercise("Рисование цветных букв и слов",
               "Визуализация текста различных цветов на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 18;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void ColoredTextDrawing::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Нарисуйте букву 'А' красным цветом.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Нарисуйте букву 'Б' синим цветом.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Напишите своё имя разными цветами (каждая буква своим).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Нарисуйте слово 'МИР' радужными цветами.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ColoredTextDrawing::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +48.0;
    m_target_metrics.concentration_change = +45.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 72.0;
    m_target_metrics.concentration_min = 75.0;
}

} // namespace Bronnikov
