#include "FastWind.h"

namespace Bronnikov {

FastWind::FastWind()
    : Exercise("Быстрый ветер",
               "Развитие динамического управления энергетическими потоками. "
               "Практикующий создает быстрые, вихревые движения энергии вокруг тела.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 5;  // Пятое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void FastWind::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Встаньте или сядьте свободно, руки вдоль тела", 10, ExercisePhase::Preparation},
        {"Закройте глаза и сосредоточьтесь", 10, ExercisePhase::Preparation},
        {"Сделайте несколько энергичных вдохов", 15, ExercisePhase::Preparation},
        {"Почувствуйте пространство вокруг своего тела", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте вихрь энергии, начинающийся у ваших ног", 25, ExercisePhase::Execution},
        {"Почувствуйте, как вихрь быстро поднимается вверх", 25, ExercisePhase::Execution},
        {"Ускорьте движение энергии, создавая спираль вокруг тела", 30, ExercisePhase::Execution},
        {"Измените направление: вихрь движется сверху вниз", 25, ExercisePhase::Execution},
        {"Создайте несколько вихрей одновременно", 30, ExercisePhase::Execution},
        {"Ощутите быстрое движение энергии, как порывы ветра", 30, ExercisePhase::Execution},
        {"Управляйте скоростью вихрей силой мысли", 25, ExercisePhase::Execution},
        {"Расширьте вихри за пределы тела", 30, ExercisePhase::Execution},
        {"Почувствуйте себя центром энергетического торнадо", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно замедляйте движение вихрей", 20, ExercisePhase::Completion},
        {"Соберите энергию обратно в центр тела", 15, ExercisePhase::Completion},
        {"Сделайте несколько спокойных вдохов", 15, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void FastWind::initializeTargetMetrics() {
    // Целевое состояние: динамичная концентрация с контролем энергии
    // Упражнение требует активного управления быстрыми процессами

    m_target_metrics.alpha_change = 12.0;           // +12% (контролируемая энергия)
    m_target_metrics.beta_change = 18.0;            // +18% (динамичная концентрация)
    m_target_metrics.theta_change = -8.0;           // -8% (активное состояние)

    m_target_metrics.concentration_change = 20.0;   // +20% (управление потоками)
    m_target_metrics.relaxation_change = 8.0;       // +8% (контролируемое движение)

    // Веса для scoring - акцент на концентрации и управлении
    m_target_metrics.alpha_weight = 0.8;
    m_target_metrics.beta_weight = 1.2;             // Бета активность важна
    m_target_metrics.theta_weight = 0.4;
    m_target_metrics.concentration_weight = 1.2;    // Концентрация важна
    m_target_metrics.relaxation_weight = 0.6;
}

} // namespace Bronnikov
