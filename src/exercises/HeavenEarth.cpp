#include "HeavenEarth.h"

namespace Bronnikov {

HeavenEarth::HeavenEarth()
    : Exercise("Небо и Земля",
               "Развитие вертикального энергетического потока. "
               "Практикующий создает связь между земной энергией (снизу) "
               "и космической энергией (сверху).")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 8;  // Восьмое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void HeavenEarth::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Встаньте прямо, ноги на ширине плеч", 10, ExercisePhase::Preparation},
        {"Руки свободно висят вдоль тела", 10, ExercisePhase::Preparation},
        {"Закройте глаза и выровняйте дыхание", 20, ExercisePhase::Preparation},
        {"Почувствуйте вертикальную ось вашего тела", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте корни, идущие от ваших ног в глубь земли", 30, ExercisePhase::Execution},
        {"Почувствуйте плотную, теплую энергию Земли", 25, ExercisePhase::Execution},
        {"Втяните земную энергию вверх через ноги", 30, ExercisePhase::Execution},
        {"Энергия поднимается по позвоночнику к макушке", 30, ExercisePhase::Execution},
        {"Представьте световой столб над вашей головой, уходящий в небо", 30, ExercisePhase::Execution},
        {"Почувствуйте легкую, светлую космическую энергию", 25, ExercisePhase::Execution},
        {"Втяните небесную энергию вниз через макушку", 30, ExercisePhase::Execution},
        {"Энергия опускается по позвоночнику вниз к стопам", 30, ExercisePhase::Execution},
        {"Создайте непрерывный поток: Земля → тело → Небо", 35, ExercisePhase::Execution},
        {"Почувствуйте себя проводником между двумя источниками", 40, ExercisePhase::Execution},

        // Completion Phase
        {"Замедлите движение энергетического потока", 20, ExercisePhase::Completion},
        {"Соберите энергию в центре тела (область пупка)", 20, ExercisePhase::Completion},
        {"Поблагодарите Небо и Землю", 10, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void HeavenEarth::initializeTargetMetrics() {
    // Целевое состояние: медитативное состояние проводника энергии
    // Упражнение требует глубокой релаксации и открытости

    m_target_metrics.alpha_change = 28.0;           // +28% (медитативное состояние)
    m_target_metrics.beta_change = 5.0;             // +5% (легкая концентрация)
    m_target_metrics.theta_change = 15.0;           // +15% (глубокая медитация)

    m_target_metrics.concentration_change = 15.0;   // +15% (удержание потока)
    m_target_metrics.relaxation_change = 32.0;      // +32% (очень высокая релаксация)

    // Веса для scoring - акцент на медитативном состоянии
    m_target_metrics.alpha_weight = 1.4;            // Альфа критична
    m_target_metrics.beta_weight = 0.4;
    m_target_metrics.theta_weight = 1.0;            // Тета важна
    m_target_metrics.concentration_weight = 0.8;
    m_target_metrics.relaxation_weight = 1.4;       // Релаксация критична
}

} // namespace Bronnikov
