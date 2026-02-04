#include "ImageProcessing.h"

namespace Bronnikov {

ImageProcessing::ImageProcessing()
    : Exercise("Обработка изображений",
               "Загрузка, трансформация и анализ изображений в биокомпьютере.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 6;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void ImageProcessing::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Загрузите изображение (фото).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Увеличьте изображение, рассмотрите детали.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Измените цвета изображения (сделайте ч/б).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Поверните изображение на 90 градусов.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Извлеките отдельный элемент изображения.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ImageProcessing::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +54.0;
    m_target_metrics.concentration_change = +50.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 78.0;
    m_target_metrics.concentration_min = 80.0;
}

} // namespace Bronnikov
