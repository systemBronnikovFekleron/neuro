#include "TextRotation.h"

namespace Bronnikov {

TextRotation::TextRotation()
    : Exercise("Повороты текста",
               "Вращение и трансформация текстовых образов на экране ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 19;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void TextRotation::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Визуализируйте слово 'ДОМ'.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Поверните слово на 90 градусов вправо.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Поверните на 180 градусов (вверх ногами).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Поверните на 270 градусов (влево).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Верните в исходное положение.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void TextRotation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +45.0;
    m_target_metrics.concentration_change = +42.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 70.0;
}

} // namespace Bronnikov
