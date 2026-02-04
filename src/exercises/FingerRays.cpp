#include "FingerRays.h"

namespace Bronnikov {

FingerRays::FingerRays()
    : Exercise("Лучи из пальцев",
               "Развитие точечных фантомных ощущений в кончиках пальцев. "
               "Практикующий создает ощущение лучей энергии, направленных "
               "из кончиков пальцев.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 3;  // Третье упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void FingerRays::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Сядьте удобно с прямой спиной", 10, ExercisePhase::Preparation},
        {"Положите руки на колени ладонями вверх", 10, ExercisePhase::Preparation},
        {"Закройте глаза и сделайте несколько глубоких вдохов", 20, ExercisePhase::Preparation},
        {"Сосредоточьте внимание на кончиках пальцев", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте точки света в кончиках пальцев", 20, ExercisePhase::Execution},
        {"Почувствуйте легкое покалывание в кончиках пальцев", 20, ExercisePhase::Execution},
        {"Представьте, как из каждого пальца исходит тонкий луч света", 30, ExercisePhase::Execution},
        {"Усильте ощущение лучей, сделайте их более яркими", 25, ExercisePhase::Execution},
        {"Направьте лучи вперед, ощущайте их протяженность", 30, ExercisePhase::Execution},
        {"Поочередно активируйте лучи: большой палец, указательный...", 40, ExercisePhase::Execution},
        {"Попробуйте соединить лучи правой и левой руки", 30, ExercisePhase::Execution},
        {"Управляйте направлением лучей движением мысли", 30, ExercisePhase::Execution},
        {"Увеличьте интенсивность всех лучей одновременно", 25, ExercisePhase::Execution},
        {"Экспериментируйте с толщиной и яркостью лучей", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно уменьшайте интенсивность лучей", 20, ExercisePhase::Completion},
        {"Соберите энергию обратно в кончики пальцев", 15, ExercisePhase::Completion},
        {"Сожмите и разожмите кулаки несколько раз", 15, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void FingerRays::initializeTargetMetrics() {
    // Целевое состояние: высокая концентрация и фокус
    // Упражнение требует точного контроля и пристального внимания

    m_target_metrics.alpha_change = 5.0;            // +5% (минимальная релаксация)
    m_target_metrics.beta_change = 15.0;            // +15% (высокая концентрация)
    m_target_metrics.theta_change = -5.0;           // -5% (снижение сонливости)

    m_target_metrics.concentration_change = 25.0;   // +25% (высокая концентрация)
    m_target_metrics.relaxation_change = 5.0;       // +5% (минимальная)

    // Веса для scoring - акцент на концентрации и фокусе
    m_target_metrics.alpha_weight = 0.5;            // Релаксация менее важна
    m_target_metrics.beta_weight = 1.5;             // Концентрация очень важна
    m_target_metrics.theta_weight = 0.3;
    m_target_metrics.concentration_weight = 1.5;    // Концентрация критична
    m_target_metrics.relaxation_weight = 0.3;       // Релаксация вторична
}

} // namespace Bronnikov
