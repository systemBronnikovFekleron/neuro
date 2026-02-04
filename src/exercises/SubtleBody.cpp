#include "SubtleBody.h"

namespace Bronnikov {

SubtleBody::SubtleBody()
    : Exercise("Тонкоматериальное тело",
               "Развитие ощущения всего энергетического тела целиком. "
               "Практикующий ощущает энергетическую оболочку, окружающую физическое тело.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 7;  // Седьмое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void SubtleBody::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Примите удобное положение лежа на спине", 15, ExercisePhase::Preparation},
        {"Закройте глаза и полностью расслабьтесь", 15, ExercisePhase::Preparation},
        {"Сделайте серию глубоких дыханий", 20, ExercisePhase::Preparation},
        {"Почувствуйте вес своего тела", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте легкое свечение на поверхности вашей кожи", 30, ExercisePhase::Execution},
        {"Почувствуйте, как это свечение расширяется на 5-10 см от тела", 30, ExercisePhase::Execution},
        {"Ощутите границу вашего энергетического тела", 30, ExercisePhase::Execution},
        {"Сканируйте вниманием всю поверхность энергетической оболочки", 35, ExercisePhase::Execution},
        {"Почувствуйте плотность и температуру тонкого тела", 30, ExercisePhase::Execution},
        {"Расширьте границы еще на 10-20 см", 30, ExercisePhase::Execution},
        {"Ощутите себя как сферу энергии", 30, ExercisePhase::Execution},
        {"Почувствуйте пульсацию энергии в тонком теле", 30, ExercisePhase::Execution},
        {"Осознайте единство физического и тонкого тел", 35, ExercisePhase::Execution},
        {"Побудьте в этом состоянии целостного восприятия", 40, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно сужайте границы энергетической оболочки", 20, ExercisePhase::Completion},
        {"Верните энергию к поверхности физического тела", 15, ExercisePhase::Completion},
        {"Пошевелите пальцами рук и ног", 10, ExercisePhase::Completion},
        {"Медленно откройте глаза", 15, ExercisePhase::Completion},
    };
}

void SubtleBody::initializeTargetMetrics() {
    // Целевое состояние: глубокая осознанность и целостное восприятие
    // Упражнение требует баланса между концентрацией и расслаблением

    m_target_metrics.alpha_change = 25.0;           // +25% (глубокая осознанность)
    m_target_metrics.beta_change = 8.0;             // +8% (легкая концентрация)
    m_target_metrics.theta_change = 12.0;           // +12% (медитативное состояние)

    m_target_metrics.concentration_change = 18.0;   // +18% (целостное восприятие)
    m_target_metrics.relaxation_change = 28.0;      // +28% (высокая релаксация)

    // Веса для scoring - баланс всех показателей
    m_target_metrics.alpha_weight = 1.3;            // Альфа очень важна
    m_target_metrics.beta_weight = 0.6;
    m_target_metrics.theta_weight = 0.8;            // Тета важна
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.3;       // Релаксация очень важна
}

} // namespace Bronnikov
