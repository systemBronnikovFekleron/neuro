#include "CreativeVisualization.h"

namespace Bronnikov {

CreativeVisualization::CreativeVisualization()
    : Exercise("Творческая визуализация",
               "Создание оригинальных художественных образов и композиций в биокомпьютере.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 10;
    m_duration_minutes = 16;
    initializeInstructions();
    initializeTargetMetrics();
}

void CreativeVisualization::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Создайте абстрактную композицию.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте цвета и формы по вдохновению.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте пейзаж (горы, река, небо).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте детали: облака, деревья, птиц.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Сохраните образ в памяти биокомпьютера.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void CreativeVisualization::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +60.0;
    m_target_metrics.theta_change = +40.0;
    m_target_metrics.concentration_change = +50.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 0.8;
    m_target_metrics.alpha_min = 82.0;
}

} // namespace Bronnikov
