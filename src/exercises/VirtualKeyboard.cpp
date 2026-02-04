#include "VirtualKeyboard.h"

namespace Bronnikov {

VirtualKeyboard::VirtualKeyboard()
    : Exercise("Виртуальная клавиатура",
               "Создание и использование виртуальной клавиатуры на экране ЛБК для ввода информации.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 3;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void VirtualKeyboard::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Визуализируйте клавиатуру на экране.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Мысленно нажмите несколько букв по очереди.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Наберите простое слово (например, 'МИР').", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Наберите короткое предложение.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void VirtualKeyboard::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +48.0;
    m_target_metrics.concentration_change = +45.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 72.0;
    m_target_metrics.concentration_min = 75.0;
}

} // namespace Bronnikov
