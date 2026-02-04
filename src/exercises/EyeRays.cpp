#include "EyeRays.h"

namespace Bronnikov {

EyeRays::EyeRays()
    : Exercise("Лучи из глаз",
               "Развитие визуальных фантомных ощущений. "
               "Практикующий создает ощущение лучей энергии, направленных из глаз.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 6;  // Шестое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void EyeRays::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Сядьте удобно с прямой спиной", 10, ExercisePhase::Preparation},
        {"Держите глаза открытыми, взгляд направлен вперед", 10, ExercisePhase::Preparation},
        {"Расслабьте лицо и глазные мышцы", 15, ExercisePhase::Preparation},
        {"Сосредоточьтесь на области вокруг глаз", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте точки света в центре каждого глаза", 20, ExercisePhase::Execution},
        {"Почувствуйте легкое тепло в области глаз", 20, ExercisePhase::Execution},
        {"Визуализируйте два луча света, исходящих из глаз", 30, ExercisePhase::Execution},
        {"Направьте лучи на воображаемую точку перед собой", 25, ExercisePhase::Execution},
        {"Усильте яркость и четкость лучей", 25, ExercisePhase::Execution},
        {"Медленно перемещайте точку фокуса лучей в пространстве", 30, ExercisePhase::Execution},
        {"Расширьте лучи, сделайте их шире", 25, ExercisePhase::Execution},
        {"Попробуйте управлять каждым лучом независимо", 30, ExercisePhase::Execution},
        {"Соедините лучи в одну мощную световую колонну", 30, ExercisePhase::Execution},
        {"Ощутите связь между взглядом и энергией", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно уменьшайте интенсивность лучей", 20, ExercisePhase::Completion},
        {"Верните энергию обратно в область глаз", 15, ExercisePhase::Completion},
        {"Мягко поморгайте несколько раз", 10, ExercisePhase::Completion},
        {"Закройте глаза и отдохните", 15, ExercisePhase::Completion},
    };
}

void EyeRays::initializeTargetMetrics() {
    // Целевое состояние: высокая визуальная концентрация
    // Упражнение требует точного контроля и визуального внимания

    m_target_metrics.alpha_change = 8.0;            // +8% (умеренная релаксация)
    m_target_metrics.beta_change = 16.0;            // +16% (визуальная концентрация)
    m_target_metrics.theta_change = -5.0;           // -5% (активное состояние)

    m_target_metrics.concentration_change = 22.0;   // +22% (визуальный контроль)
    m_target_metrics.relaxation_change = 10.0;      // +10% (спокойная концентрация)

    // Веса для scoring - акцент на концентрации и фокусе
    m_target_metrics.alpha_weight = 0.6;
    m_target_metrics.beta_weight = 1.3;             // Бета важна
    m_target_metrics.theta_weight = 0.3;
    m_target_metrics.concentration_weight = 1.3;    // Концентрация критична
    m_target_metrics.relaxation_weight = 0.5;
}

} // namespace Bronnikov
