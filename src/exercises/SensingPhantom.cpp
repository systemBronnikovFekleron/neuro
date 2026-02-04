#include "SensingPhantom.h"

namespace Bronnikov {

SensingPhantom::SensingPhantom()
    : Exercise("Ощущающий фантом",
               "Развитие виртуального тела, способного ощущать пространство. "
               "Расширение фантома для исследования окружающей среды.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 3;
    m_duration_minutes = 10;

    initializeInstructions();
    initializeTargetMetrics();
}

void SensingPhantom::initializeInstructions() {
    m_instructions.push_back({
        "Встаньте, руки опущены. Закройте глаза. Дышите спокойно. "
        "Записываются базовые показатели.",
        30,
        ExercisePhase::Baseline
    });

    m_instructions.push_back({
        "Мысленно поднимите правую виртуальную руку в сторону. "
        "Удлините её до правой стены и ощупайте стену, не поворачивая головы.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Аналогично ощупайте левой виртуальной рукой левую стену. "
        "Запомните ощущения от прикосновения к разным поверхностям.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Наблюдайте ощущение пола под ногами. Сделайте шаг вперёд виртуальной ногой "
        "и перенесите центр тяжести виртуального тела. Походите по комнате мысленно.",
        120,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Удлините виртуальную шею и достаньте теменной частью головы до потолка. "
        "Ощутите поверхность потолка.",
        90,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Заполните своим виртуальным телом всё пространство комнаты. "
        "Стремитесь ощутить одновременно все внутренние поверхности.",
        120,
        ExercisePhase::Execution
    });

    m_instructions.push_back({
        "Верните фантом в обычное состояние. Дышите спокойно. "
        "Записываются финальные показатели.",
        30,
        ExercisePhase::Post
    });
}

void SensingPhantom::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +30.0;
    m_target_metrics.theta_change = +20.0;
    m_target_metrics.concentration_change = +10.0;
    m_target_metrics.relaxation_change = +15.0;

    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 0.6;
    m_target_metrics.relaxation_weight = 0.8;

    m_target_metrics.alpha_min = 50.0;
    m_target_metrics.alpha_max = 75.0;
}

double SensingPhantom::checkTargetState(const MetricsSnapshot& metrics) const {
    double score = 0.0;
    double total_weight = 0.0;

    if (metrics.alpha_power >= m_target_metrics.alpha_min &&
        metrics.alpha_power <= m_target_metrics.alpha_max) {
        score += 100.0 * m_target_metrics.alpha_weight;
    } else if (metrics.alpha_power >= m_target_metrics.alpha_min - 10.0) {
        score += 70.0 * m_target_metrics.alpha_weight;
    }
    total_weight += m_target_metrics.alpha_weight;

    if (metrics.relaxation >= 60.0) {
        score += 100.0 * m_target_metrics.relaxation_weight;
    } else if (metrics.relaxation >= 50.0) {
        score += 70.0 * m_target_metrics.relaxation_weight;
    }
    total_weight += m_target_metrics.relaxation_weight;

    return (total_weight > 0.0) ? (score / total_weight) : 0.0;
}

} // namespace Bronnikov
