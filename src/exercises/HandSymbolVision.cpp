#include "HandSymbolVision.h"

namespace Bronnikov {

HandSymbolVision::HandSymbolVision()
    : Exercise("Видеть рукой простые знаки",
               "Распознавание геометрических фигур и простых символов через тактильный контакт.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 12;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void HandSymbolVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Положите руку на карточку с символом.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Ощутите форму через биоэнергетический контакт.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Визуализируйте символ на экране ЛБК.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Назовите фигуру: круг, квадрат, треугольник.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте с 5-6 разными символами.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void HandSymbolVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +42.0;
    m_target_metrics.concentration_change = +40.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 68.0;
    m_target_metrics.concentration_min = 72.0;
    m_target_metrics.focus_min = 70.0;
}

} // namespace Bronnikov
