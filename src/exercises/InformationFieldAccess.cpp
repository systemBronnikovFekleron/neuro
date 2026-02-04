#include "InformationFieldAccess.h"

namespace Bronnikov {

InformationFieldAccess::InformationFieldAccess()
    : Exercise("Доступ к информационному полю",
               "Подключение к глобальному информационному полю для получения знаний.")
{
    m_stage = ExerciseStage::Stage4;
    m_order_in_stage = 7;
    m_duration_minutes = 22;
    initializeInstructions();
    initializeTargetMetrics();
}

void InformationFieldAccess::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Расширьте сознание до глобального уровня.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Настройтесь на информационное поле Земли.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Задайте вопрос, на который ищете ответ.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Примите информацию: образы, символы, ощущения.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Интерпретируйте полученное через биокомпьютер.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК постепенно.", 30, ExercisePhase::Post});
}

void InformationFieldAccess::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +78.0;
    m_target_metrics.theta_change = +60.0;
    m_target_metrics.concentration_change = +72.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 98.0;
    m_target_metrics.theta_min = 45.0;
}

} // namespace Bronnikov
