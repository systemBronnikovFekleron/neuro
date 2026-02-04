#include "EnergyHandsStage1.h"

namespace Bronnikov {

EnergyHandsStage1::EnergyHandsStage1()
    : Exercise("Энергетические руки",
               "Развитие управления виртуальными (энергетическими) руками. "
               "Манипуляции с окружающим пространством энергетическими руками.")
{
    // Маркируем как 1-я ступень (Экология духа), упражнение №2
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 2;

    m_duration_minutes = 7;  // 5-10 минут
    initializeInstructions();
    initializeTargetMetrics();
}

void EnergyHandsStage1::initializeInstructions() {
    // Baseline фаза (30 сек)
    m_instructions.push_back({
        "Встаньте, ноги на ширине плеч, руки опущены. Закройте глаза. "
        "Дышите спокойно. Записываются базовые показатели.",
        30,
        ExercisePhase::Baseline
    });

    // Основная фаза
    m_instructions.push_back({
        "Разведите руки в стороны, затем сведите перед собой, "
        "затем поднимите кверху и опустите вниз. Запомните ощущения.",
        60,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Теперь мысленно разводите в стороны не физические руки, а виртуальные. "
        "Обратите внимание на сохранение ощущений.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Мысленно сводите энергетические руки перед собой и поднимайте их кверху, "
        "затем опускайте вниз. Чувствуйте энергетические ощущения.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Проделайте энергетическими руками различные манипуляции. "
        "Удлиняйте их, трогайте стены, потолок комнаты.",
        90,
        ExercisePhase::Execution
    });

    // Post фаза (30 сек)
    m_instructions.push_back({
        "Верните энергетические руки в обычное состояние. "
        "Дышите спокойно. Записываются финальные показатели.",
        30,
        ExercisePhase::Post
    });
}

void EnergyHandsStage1::initializeTargetMetrics() {
    // Целевые изменения для работы с энергетическими руками
    m_target_metrics.alpha_change = +20.0;
    m_target_metrics.concentration_change = +15.0;
    m_target_metrics.relaxation_change = +10.0;

    // Веса метрик
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 0.8;
    m_target_metrics.relaxation_weight = 0.6;

    // Диапазоны
    m_target_metrics.alpha_min = 45.0;
    m_target_metrics.alpha_max = 70.0;
    m_target_metrics.concentration_min = 50.0;
    m_target_metrics.concentration_max = 75.0;
}

double EnergyHandsStage1::checkTargetState(const MetricsSnapshot& metrics) const {
    double score = 0.0;
    double total_weight = 0.0;

    // Alpha (внутренний фокус на энергетических ощущениях)
    if (metrics.alpha_power >= m_target_metrics.alpha_min &&
        metrics.alpha_power <= m_target_metrics.alpha_max) {
        score += 100.0 * m_target_metrics.alpha_weight;
    } else if (metrics.alpha_power >= m_target_metrics.alpha_min - 10.0) {
        score += 70.0 * m_target_metrics.alpha_weight;
    }
    total_weight += m_target_metrics.alpha_weight;

    // Concentration (удержание внимания на виртуальных руках)
    if (metrics.concentration >= m_target_metrics.concentration_min) {
        score += 100.0 * m_target_metrics.concentration_weight;
    } else if (metrics.concentration >= m_target_metrics.concentration_min - 10.0) {
        score += 70.0 * m_target_metrics.concentration_weight;
    }
    total_weight += m_target_metrics.concentration_weight;

    return (total_weight > 0.0) ? (score / total_weight) : 0.0;
}

} // namespace Bronnikov
