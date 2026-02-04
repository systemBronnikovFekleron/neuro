#include "InformationLoading.h"

namespace Bronnikov {

InformationLoading::InformationLoading()
    : Exercise("Загрузка информации в биокомпьютер",
               "Техника быстрой загрузки текстовой и визуальной информации в ЛБК.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 2;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void InformationLoading::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК и биокомпьютер.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Положите руку на страницу текста.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Мысленно скопируйте страницу на экран ЛБК.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Прочитайте текст на экране ЛБК.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Повторите с другой страницей.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void InformationLoading::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +55.0;
    m_target_metrics.concentration_change = +50.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 78.0;
    m_target_metrics.concentration_min = 80.0;
}

} // namespace Bronnikov
