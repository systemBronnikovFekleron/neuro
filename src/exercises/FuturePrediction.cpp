#include "FuturePrediction.h"

namespace Bronnikov {

FuturePrediction::FuturePrediction()
    : Exercise("Прогнозирование будущего",
               "Моделирование вероятных сценариев развития событий через биокомпьютер.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 13;
    m_duration_minutes = 16;
    initializeInstructions();
    initializeTargetMetrics();
}

void FuturePrediction::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Загрузите текущую ситуацию.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Определите ключевые переменные (факторы влияния).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Запросите прогноз на ближайшую неделю.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите 2-3 вероятных сценария.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Оцените вероятность каждого сценария.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void FuturePrediction::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +65.0;
    m_target_metrics.theta_change = +45.0;
    m_target_metrics.concentration_change = +55.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 88.0;
}

} // namespace Bronnikov
