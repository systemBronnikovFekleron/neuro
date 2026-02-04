#include "LanguageTranslation.h"

namespace Bronnikov {

LanguageTranslation::LanguageTranslation()
    : Exercise("Перевод языков",
               "Использование биокомпьютера для перевода слов и фраз между языками.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 8;
    m_duration_minutes = 14;
    initializeInstructions();
    initializeTargetMetrics();
}

void LanguageTranslation::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Загрузите русское слово.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Запросите перевод на английский. Биокомпьютер показывает.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте с другими словами (5-10 слов).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Загрузите короткую фразу, получите перевод.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Проверьте правильность переводов.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void LanguageTranslation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +52.0;
    m_target_metrics.theta_change = +32.0;
    m_target_metrics.concentration_change = +48.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.7;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 76.0;
}

} // namespace Bronnikov
