#include "ScreenColoring.h"

namespace Bronnikov {

ScreenColoring::ScreenColoring()
    : Exercise("Раскраска экрана",
               "Изменение цвета экрана ЛБК. Развитие управления визуализацией.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 5;
    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void ScreenColoring::initializeInstructions() {
    m_instructions.push_back({
        "Поза 'кучера', глаза закрыты повязкой. Записываются базовые показатели.",
        30, ExercisePhase::Baseline
    });

    m_instructions.push_back({
        "Включите ЛБК. Белый экран перед вами.", 60, ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Измените цвет экрана на синий. Яркий, насыщенный синий цвет.", 90, ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Синий экран меняем на красный. Чистый красный цвет.", 90, ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Красный экран меняем на зелёный. Яркий зелёный.", 90, ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Зелёный экран возвращаем к белому. Выключите ЛБК.", 30, ExercisePhase::Post
    });
}

void ScreenColoring::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +30.0;
    m_target_metrics.concentration_change = +20.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.8;
    m_target_metrics.alpha_min = 55.0;
    m_target_metrics.alpha_max = 80.0;
}

double ScreenColoring::checkTargetState(const MetricsSnapshot& metrics) const {
    double score = 0.0;
    double total_weight = 0.0;

    if (metrics.alpha_power >= m_target_metrics.alpha_min) {
        score += 100.0 * m_target_metrics.alpha_weight;
    } else {
        score += (metrics.alpha_power / m_target_metrics.alpha_min) * 70.0 * m_target_metrics.alpha_weight;
    }
    total_weight += m_target_metrics.alpha_weight;

    if (metrics.concentration >= 55.0) {
        score += 100.0 * m_target_metrics.concentration_weight;
    }
    total_weight += m_target_metrics.concentration_weight;

    return (total_weight > 0.0) ? (score / total_weight) : 0.0;
}

} // namespace Bronnikov
