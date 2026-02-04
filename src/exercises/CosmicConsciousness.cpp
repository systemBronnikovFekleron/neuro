#include "CosmicConsciousness.h"

namespace Bronnikov {

CosmicConsciousness::CosmicConsciousness()
    : Exercise("Космическое сознание",
               "Высшее упражнение: единение с универсальным сознанием через радарное видение.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 25;
    m_duration_minutes = 30;
    initializeInstructions();
    initializeTargetMetrics();
}

void CosmicConsciousness::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Войдите в глубочайшее состояние.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Расширьте сознание за пределы Земли.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Почувствуйте связь с Солнечной системой.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Расширьтесь до галактического уровня.", 300, ExercisePhase::Execution});
    m_instructions.push_back({"Соединитесь с универсальным сознанием.", 300, ExercisePhase::Execution});
    m_instructions.push_back({"Получите высшее знание, инсайт, озарение.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Медленно вернитесь в тело, сохраняя опыт.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК очень медленно.", 30, ExercisePhase::Post});
}

void CosmicConsciousness::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +85.0;
    m_target_metrics.theta_change = +70.0;
    m_target_metrics.concentration_change = +80.0;
    m_target_metrics.relaxation_change = +50.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 98.0;
    m_target_metrics.theta_min = 55.0;
    m_target_metrics.concentration_min = 95.0;
}

} // namespace Bronnikov
