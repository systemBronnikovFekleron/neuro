#include "InformationSearch.h"

namespace Bronnikov {

InformationSearch::InformationSearch()
    : Exercise("Поиск информации в биокомпьютере",
               "Техника быстрого поиска загруженной информации через запросы к ЛБК.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 4;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void InformationSearch::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК и биокомпьютер.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Загрузите небольшой текст в биокомпьютер.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Задайте вопрос: 'Где упоминается слово X?'", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Биокомпьютер выделяет нужный фрагмент на экране.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте найти другие слова или фразы.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void InformationSearch::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +52.0;
    m_target_metrics.concentration_change = +48.0;
    m_target_metrics.theta_change = +30.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 76.0;
}

} // namespace Bronnikov
