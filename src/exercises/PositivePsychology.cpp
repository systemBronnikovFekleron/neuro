#include "PositivePsychology.h"

namespace Bronnikov {

PositivePsychology::PositivePsychology()
    : Exercise("Позитивная психологическая настройка",
               "Приведение себя в рабочее состояние через позитивный настрой. "
               "Базовое упражнение перед работой с ЛБК.")
{
    // Маркируем как 1-я ступень, упражнение №1
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 1;

    m_duration_minutes = 5;  // 3-5 минут
    initializeInstructions();
    initializeTargetMetrics();
}

void PositivePsychology::initializeInstructions() {
    // Baseline фаза (30 сек)
    m_instructions.push_back({
        "Сядьте удобно. Закройте глаза. Дышите спокойно и ровно. "
        "Сейчас записываются ваши базовые показатели в обычном состоянии.",
        30,
        ExercisePhase::Baseline
    });

    // Основная фаза (4 минуты)
    m_instructions.push_back({
        "Поднимите руки вверх и в стороны, потянитесь и прогнитесь. "
        "С удовольствием зевните, представляя, что только что проснулись после приятного сна.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "На лице изобразите самую широкую улыбку. "
        "Представьте перед собой огромное теплое ласковое солнце.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Почувствуйте себя самым счастливым человеком - самым здоровым и сильным, "
        "самым богатым, самым умным и красивым, самым свободным и радостным.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Вокруг себя увидьте только добрых, прекрасных и замечательных друзей. "
        "Почувствуйте глубокую благодарность к миру.",
        60,
        ExercisePhase::Execution
    });

    // Post фаза (30 сек)
    m_instructions.push_back({
        "Сделайте пару раз глубокий вдох и спокойный выдох. "
        "Сохраните это состояние. Записываются финальные показатели.",
        30,
        ExercisePhase::Post
    });
}

void PositivePsychology::initializeTargetMetrics() {
    // Целевые изменения относительно baseline
    m_target_metrics.alpha_change = +25.0;      // Увеличение alpha на 25%
    m_target_metrics.beta_change = -15.0;       // Снижение beta на 15%
    m_target_metrics.theta_change = +10.0;      // Увеличение theta на 10%
    m_target_metrics.relaxation_change = +30.0; // Увеличение relaxation на 30%
    m_target_metrics.concentration_change = -10.0; // Небольшое снижение concentration

    // Веса метрик (важность для данного упражнения)
    m_target_metrics.alpha_weight = 1.0;        // Alpha - главная метрика
    m_target_metrics.beta_weight = 0.5;
    m_target_metrics.theta_weight = 0.7;
    m_target_metrics.relaxation_weight = 1.0;   // Relaxation - ключевая метрика
    m_target_metrics.concentration_weight = 0.3;

    // Диапазоны для основных метрик (абсолютные значения)
    m_target_metrics.relaxation_min = 70.0;
    m_target_metrics.relaxation_max = 90.0;
}

double PositivePsychology::checkTargetState(const MetricsSnapshot& metrics) const {
    // Для позитивной настройки ключевые показатели - Alpha и Relaxation
    double score = 0.0;
    double total_weight = 0.0;

    // Проверка Relaxation (самый важный показатель)
    if (metrics.relaxation >= m_target_metrics.relaxation_min &&
        metrics.relaxation <= m_target_metrics.relaxation_max) {
        score += 100.0 * 1.0;  // Полный балл
    } else if (metrics.relaxation >= m_target_metrics.relaxation_min - 10.0) {
        score += 70.0 * 1.0;   // Близко к цели
    } else {
        score += (metrics.relaxation / m_target_metrics.relaxation_min) * 50.0 * 1.0;
    }
    total_weight += 1.0;

    // Проверка Alpha (второй важный показатель)
    if (metrics.alpha_power >= 50.0) {  // Высокий alpha
        score += 100.0 * 0.8;
    } else if (metrics.alpha_power >= 40.0) {
        score += 70.0 * 0.8;
    } else {
        score += (metrics.alpha_power / 50.0) * 50.0 * 0.8;
    }
    total_weight += 0.8;

    // Проверка Beta (должен быть низким для релаксации)
    if (metrics.beta_power <= 30.0) {  // Низкий beta
        score += 100.0 * 0.5;
    } else if (metrics.beta_power <= 40.0) {
        score += 70.0 * 0.5;
    } else {
        score += (50.0 - metrics.beta_power) * 0.5;
    }
    total_weight += 0.5;

    return (total_weight > 0.0) ? (score / total_weight) : 0.0;
}

} // namespace Bronnikov
