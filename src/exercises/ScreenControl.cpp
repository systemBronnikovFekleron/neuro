#include "ScreenControl.h"

namespace Bronnikov {

ScreenControl::ScreenControl()
    : Exercise("Управление экраном ЛБК",
               "Включение и выключение экрана личного биокомпьютера. "
               "Фундаментальное упражнение для развития зрения вне глаз.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 4;
    m_duration_minutes = 8;

    initializeInstructions();
    initializeTargetMetrics();
}

void ScreenControl::initializeInstructions() {
    m_instructions.push_back({
        "Примите позу 'кучера'. Глаза закрыты повязкой. Дышите спокойно. "
        "Записываются базовые показатели.",
        30,
        ExercisePhase::Baseline
    });

    m_instructions.push_back({
        "Внимательно рассмотрите темное комфортное пространство перед закрытыми глазами. "
        "Это ваш черный экран - основа для работы.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Представьте белую точку в центре темного пространства. "
        "Удерживайте её, сделайте яркой и чёткой.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Разверните точку в горизонтальную белую линию. "
        "Линия должна быть яркой и ровной.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Раскатайте линию вертикально, превращая её в белый экран. "
        "Это и есть включение вашего ЛБК. Экран должен быть ровным и ярким.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Удерживайте белый экран. Наблюдайте его чёткость и яркость. "
        "Это ваш рабочий инструмент для всех последующих упражнений.",
        120,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Выключите ЛБК в обратном порядке: экран → линия → точка → темнота. "
        "Записываются финальные показатели.",
        30,
        ExercisePhase::Post
    });
}

void ScreenControl::initializeTargetMetrics() {
    // Для работы с экраном ЛБК ключевая метрика - Alpha (зрительная кора)
    m_target_metrics.alpha_change = +35.0;      // Сильное увеличение alpha
    m_target_metrics.concentration_change = +25.0;
    m_target_metrics.beta_change = -10.0;
    m_target_metrics.relaxation_change = +15.0;

    m_target_metrics.alpha_weight = 1.0;        // Alpha - главная метрика
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.beta_weight = 0.4;
    m_target_metrics.relaxation_weight = 0.6;

    m_target_metrics.alpha_min = 55.0;
    m_target_metrics.alpha_max = 80.0;
    m_target_metrics.concentration_min = 60.0;
    m_target_metrics.concentration_max = 85.0;
}

double ScreenControl::checkTargetState(const MetricsSnapshot& metrics) const {
    double score = 0.0;
    double total_weight = 0.0;

    // Alpha - ключевой показатель для ЛБК
    if (metrics.alpha_power >= m_target_metrics.alpha_min &&
        metrics.alpha_power <= m_target_metrics.alpha_max) {
        score += 100.0 * m_target_metrics.alpha_weight;
    } else if (metrics.alpha_power >= m_target_metrics.alpha_min - 10.0) {
        score += 70.0 * m_target_metrics.alpha_weight;
    }
    total_weight += m_target_metrics.alpha_weight;

    // Concentration - удержание образа экрана
    if (metrics.concentration >= m_target_metrics.concentration_min) {
        score += 100.0 * m_target_metrics.concentration_weight;
    } else if (metrics.concentration >= m_target_metrics.concentration_min - 10.0) {
        score += 70.0 * m_target_metrics.concentration_weight;
    }
    total_weight += m_target_metrics.concentration_weight;

    return (total_weight > 0.0) ? (score / total_weight) : 0.0;
}

} // namespace Bronnikov
