#include "BallMovement.h"

namespace Bronnikov {

BallMovement::BallMovement()
    : Exercise("Перемещение шара",
               "Развитие управления фантомными ощущениями через движение "
               "энергетического шара в пространстве. Практикующий перемещает "
               "энергетический шар вверх-вниз, влево-вправо, по кругу.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 2;  // Второе упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void BallMovement::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Примите удобное положение сидя с прямой спиной", 10, ExercisePhase::Preparation},
        {"Закройте глаза и расслабьтесь", 10, ExercisePhase::Preparation},
        {"Сделайте 3-5 глубоких вдохов и выдохов", 20, ExercisePhase::Preparation},
        {"Сосредоточьтесь на ощущениях в ладонях", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте энергетический шар между ладонями", 30, ExercisePhase::Execution},
        {"Медленно перемещайте шар вверх на 10-15 см", 20, ExercisePhase::Execution},
        {"Медленно опустите шар вниз", 20, ExercisePhase::Execution},
        {"Повторите движение вверх-вниз несколько раз", 30, ExercisePhase::Execution},
        {"Ощущайте тепло и покалывание при движении шара", 20, ExercisePhase::Execution},
        {"Теперь перемещайте шар влево-вправо", 30, ExercisePhase::Execution},
        {"Увеличьте амплитуду движения", 20, ExercisePhase::Execution},
        {"Попробуйте перемещать шар по кругу (по часовой стрелке)", 30, ExercisePhase::Execution},
        {"Смените направление (против часовой стрелки)", 30, ExercisePhase::Execution},
        {"Экспериментируйте с траекториями движения", 40, ExercisePhase::Execution},

        // Completion Phase
        {"Верните шар в центральное положение между ладонями", 15, ExercisePhase::Completion},
        {"Медленно уменьшайте размер шара", 20, ExercisePhase::Completion},
        {"Рассейте энергию, разведя ладони в стороны", 15, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion}
    };
}

void BallMovement::initializeTargetMetrics() {
    // Целевое состояние: баланс концентрации и релаксации
    // Упражнение требует одновременно концентрации (управление движением)
    // и релаксации (плавные движения, чувствительность)

    m_target_metrics.alpha_change = 10.0;           // +10% (умеренное повышение релаксации)
    m_target_metrics.beta_change = 5.0;             // +5% (легкая концентрация на движении)
    m_target_metrics.theta_change = 0.0;            // Не изменяется

    m_target_metrics.concentration_change = 15.0;   // +15% (управление движением)
    m_target_metrics.relaxation_change = 15.0;      // +15% (баланс!)

    // Веса для scoring - равная важность концентрации и релаксации
    m_target_metrics.alpha_weight = 1.0;            // Релаксация важна
    m_target_metrics.beta_weight = 0.5;             // Концентрация умеренно важна
    m_target_metrics.theta_weight = 0.3;            // Тета менее важна
    m_target_metrics.concentration_weight = 1.0;    // Концентрация важна
    m_target_metrics.relaxation_weight = 1.0;       // Релаксация также важна
}

} // namespace Bronnikov
