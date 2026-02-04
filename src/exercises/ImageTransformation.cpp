#include "ImageTransformation.h"

namespace Bronnikov {

ImageTransformation::ImageTransformation()
    : Exercise("Изменение размера и положения изображения",
               "Управление масштабом и позицией образов на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 17;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void ImageTransformation::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Визуализируйте простой объект (яблоко).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Увеличьте образ в 2 раза. Удерживайте.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Уменьшите образ до минимума.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Переместите образ влево, вправо, вверх, вниз.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ImageTransformation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +40.0;
    m_target_metrics.concentration_change = +38.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 65.0;
}

} // namespace Bronnikov
