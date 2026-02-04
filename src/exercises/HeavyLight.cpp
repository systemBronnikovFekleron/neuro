#include "HeavyLight.h"

namespace Bronnikov {

HeavyLight::HeavyLight()
    : Exercise("Тяжелое и Легкое",
               "Развитие контроля над восприятием веса и плотности энергии. "
               "Практикующий создает контрастные ощущения тяжести и легкости в теле.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 9;  // Девятое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void HeavyLight::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Лягте на спину, руки вдоль тела", 15, ExercisePhase::Preparation},
        {"Закройте глаза и расслабьтесь", 15, ExercisePhase::Preparation},
        {"Сделайте несколько глубоких вдохов", 20, ExercisePhase::Preparation},
        {"Почувствуйте естественный вес своего тела", 15, ExercisePhase::Preparation},

        // Execution Phase - Тяжесть
        {"Сосредоточьтесь на правой руке", 20, ExercisePhase::Execution},
        {"Представьте, что рука становится очень тяжелой", 30, ExercisePhase::Execution},
        {"Рука наливается свинцовой тяжестью, тонет в поверхности", 30, ExercisePhase::Execution},
        {"Почувствуйте, что не можете поднять эту руку", 25, ExercisePhase::Execution},

        // Execution Phase - Легкость
        {"Теперь сосредоточьтесь на левой руке", 20, ExercisePhase::Execution},
        {"Представьте, что рука становится очень легкой", 30, ExercisePhase::Execution},
        {"Рука наполняется воздухом, как воздушный шар", 30, ExercisePhase::Execution},
        {"Почувствуйте, как рука сама поднимается вверх", 25, ExercisePhase::Execution},

        // Execution Phase - Смена полярности
        {"Поменяйте ощущения: правая рука легкая, левая тяжелая", 35, ExercisePhase::Execution},
        {"Еще раз поменяйте полярность", 30, ExercisePhase::Execution},
        {"Сделайте обе руки тяжелыми одновременно", 25, ExercisePhase::Execution},
        {"Сделайте обе руки легкими одновременно", 25, ExercisePhase::Execution},
        {"Распространите ощущения на все тело", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Верните рукам естественный вес", 20, ExercisePhase::Completion},
        {"Пошевелите пальцами рук и ног", 10, ExercisePhase::Completion},
        {"Потянитесь всем телом", 10, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void HeavyLight::initializeTargetMetrics() {
    // Целевое состояние: гибкость управления состояниями
    // Упражнение требует переключения между разными режимами работы мозга

    m_target_metrics.alpha_change = 15.0;           // +15% (контроль состояний)
    m_target_metrics.beta_change = 12.0;            // +12% (управление вниманием)
    m_target_metrics.theta_change = 8.0;            // +8% (погружение в ощущения)

    m_target_metrics.concentration_change = 20.0;   // +20% (контроль переключений)
    m_target_metrics.relaxation_change = 18.0;      // +18% (расслабление для контроля)

    // Веса для scoring - баланс показателей
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.beta_weight = 1.0;             // Равный баланс
    m_target_metrics.theta_weight = 0.7;
    m_target_metrics.concentration_weight = 1.1;    // Концентрация важна
    m_target_metrics.relaxation_weight = 1.0;
}

} // namespace Bronnikov
