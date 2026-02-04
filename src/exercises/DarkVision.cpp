#include "DarkVision.h"

namespace Bronnikov {

DarkVision::DarkVision()
    : Exercise("Видеть и рисовать в темноте",
               "Использование альтернативного зрения в условиях полной темноты.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 23;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void DarkVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Погасите свет. Включите ЛБК.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите предметы в комнате через ЛБК.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Возьмите карандаш и бумагу.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Нарисуйте простую фигуру (круг, квадрат), видя через ЛБК.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Включите свет, проверьте рисунок. Выключите ЛБК.", 30, ExercisePhase::Post});
}

void DarkVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +62.0;
    m_target_metrics.concentration_change = +58.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 85.0;
    m_target_metrics.concentration_min = 85.0;
}

} // namespace Bronnikov
