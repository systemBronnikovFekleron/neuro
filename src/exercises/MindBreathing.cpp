#include "MindBreathing.h"

namespace Bronnikov {

MindBreathing::MindBreathing()
    : Exercise("Дыхание мыслью",
               "Развитие контроля за внутренними процессами через визуализацию "
               "дыхания энергией. Практикующий представляет, что дышит не воздухом, "
               "а световой энергией, которая наполняет все тело.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 4;  // Четвертое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void MindBreathing::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Примите удобное положение сидя или лежа", 15, ExercisePhase::Preparation},
        {"Закройте глаза и полностью расслабьтесь", 15, ExercisePhase::Preparation},
        {"Сделайте несколько естественных вдохов и выдохов", 20, ExercisePhase::Preparation},
        {"Почувствуйте ритм своего дыхания", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте, что вдыхаете светлую, чистую энергию", 30, ExercisePhase::Execution},
        {"На вдохе энергия входит через нос и наполняет легкие", 30, ExercisePhase::Execution},
        {"Почувствуйте, как энергия разливается по всему телу", 30, ExercisePhase::Execution},
        {"На выдохе представьте, что выдыхаете темную, использованную энергию", 30, ExercisePhase::Execution},
        {"С каждым вдохом света становится больше", 30, ExercisePhase::Execution},
        {"С каждым выдохом уходит напряжение и усталость", 30, ExercisePhase::Execution},
        {"Почувствуйте, как тело наполняется легкостью и свежестью", 30, ExercisePhase::Execution},
        {"Дышите энергией естественно, без усилий", 40, ExercisePhase::Execution},
        {"Ощутите единство с дыханием вселенной", 30, ExercisePhase::Execution},
        {"Растворитесь в потоке энергии", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно возвращайтесь к обычному дыханию", 20, ExercisePhase::Completion},
        {"Почувствуйте, как тело наполнено энергией и свежестью", 15, ExercisePhase::Completion},
        {"Сделайте несколько глубоких вдохов", 15, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void MindBreathing::initializeTargetMetrics() {
    // Целевое состояние: глубокая релаксация и медитативное состояние
    // Упражнение направлено на максимальное расслабление и отключение ментального шума

    m_target_metrics.alpha_change = 30.0;           // +30% (максимальная релаксация)
    m_target_metrics.beta_change = -15.0;           // -15% (снижение ментального шума)
    m_target_metrics.theta_change = 10.0;           // +10% (медитативное состояние)

    m_target_metrics.concentration_change = -10.0;  // -10% (отпускание контроля)
    m_target_metrics.relaxation_change = 35.0;      // +35% (очень высокая релаксация)

    // Веса для scoring - акцент на релаксации и снижении стресса
    m_target_metrics.alpha_weight = 1.5;            // Альфа критична
    m_target_metrics.beta_weight = 0.8;             // Снижение бета важно
    m_target_metrics.theta_weight = 0.7;            // Тета умеренно важна
    m_target_metrics.concentration_weight = 0.3;    // Концентрация не важна
    m_target_metrics.relaxation_weight = 1.5;       // Релаксация критична
}

} // namespace Bronnikov
