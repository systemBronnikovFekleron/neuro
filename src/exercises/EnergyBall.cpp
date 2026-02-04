#include "EnergyBall.h"
#include <cmath>

namespace Bronnikov {

EnergyBall::EnergyBall()
    : Exercise("Энергетический шар",
               "Развитие фантомных ощущений между руками. "
               "Создайте ощущение энергетического шара между ладонями.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 1;  // Первое упражнение подготовительной ступени

    m_duration_minutes = 10;
    initializeInstructions();
    initializeTargetMetrics();
}

void EnergyBall::initializeInstructions() {
    m_instructions = {
        // Подготовительная фаза
        {
            "Примите удобное положение сидя. Закройте глаза и расслабьтесь. "
            "Сделайте несколько глубоких вдохов.",
            30,
            ExercisePhase::Preparation
        },
        {
            "Потрите ладони рук друг о друга в течение 10-15 секунд, "
            "пока не почувствуете тепло.",
            20,
            ExercisePhase::Preparation
        },

        // Основная фаза
        {
            "Расположите руки перед собой на уровне груди, "
            "ладонями друг к другу на расстоянии 20-30 см.",
            15,
            ExercisePhase::Execution
        },
        {
            "Медленно сближайте и отдаляйте ладони, чувствуя пространство между ними. "
            "Представьте, что между руками находится упругий шар энергии.",
            120,
            ExercisePhase::Execution
        },
        {
            "Продолжайте работу с шаром. Сжимайте и растягивайте его, "
            "ощущайте сопротивление и упругость. Фокусируйтесь на ощущениях.",
            180,
            ExercisePhase::Execution
        },
        {
            "Попробуйте вращать шар между ладонями. "
            "Усильте концентрацию на фантомных ощущениях.",
            120,
            ExercisePhase::Execution
        },

        // Завершающая фаза
        {
            "Медленно уменьшайте размер шара, сближая ладони.",
            30,
            ExercisePhase::Completion
        },
        {
            "Положите руки на колени. Сделайте несколько глубоких вдохов. "
            "Медленно откройте глаза.",
            30,
            ExercisePhase::Completion
        }
    };
}

void EnergyBall::initializeTargetMetrics() {
    // Целевые метрики для упражнения "Энергетический шар"
    m_target_metrics.alpha_change = 15.0;          // Ожидаем +15% альфа
    m_target_metrics.beta_change = -10.0;          // Ожидаем -10% бета (меньше шума)
    m_target_metrics.concentration_change = 20.0;  // Ожидаем +20% концентрации
    m_target_metrics.relaxation_change = 10.0;     // Ожидаем +10% релаксации

    // Весовые коэффициенты важности метрик
    m_target_metrics.alpha_weight = 1.0;           // Альфа - самая важная
    m_target_metrics.concentration_weight = 1.0;   // Концентрация - очень важна
    m_target_metrics.beta_weight = 0.7;            // Бета - средняя важность
    m_target_metrics.relaxation_weight = 0.5;      // Релаксация - умеренная важность

    // Диапазоны абсолютных значений
    m_target_metrics.alpha_min = 40.0;
    m_target_metrics.alpha_max = 80.0;
    m_target_metrics.concentration_min = 50.0;
    m_target_metrics.concentration_max = 90.0;
}

void EnergyBall::start(MetricsCollector& metrics_collector) {
    // Сохраняем baseline для сравнения
    m_baseline = metrics_collector.getCurrentMetrics();

    Exercise::start(metrics_collector);
}

void EnergyBall::update(const MetricsSnapshot& current_metrics, double elapsed_seconds) {
    Exercise::update(current_metrics, elapsed_seconds);

    // Специфическая логика для упражнения "Энергетический шар"
    // Например, можно давать подсказки если метрики уходят не туда

    double target_score = checkTargetState(current_metrics);

    // Если в основной фазе и оценка низкая, можно дать подсказку
    if (m_current_phase == ExercisePhase::Execution && target_score < 30.0) {
        // Можно отправить callback с подсказкой
        // "Попробуйте больше расслабиться и усилить фокус на ощущениях"
    }
}

double EnergyBall::checkTargetState(const MetricsSnapshot& metrics) const {
    if (m_baseline.timestamp == 0.0) {
        return 0.0;  // Baseline не установлен
    }

    double score = 0.0;
    double total_weight = 0.0;

    // Проверка изменения альфа относительно baseline
    if (m_baseline.alpha_power > 0.0) {
        double alpha_change_percent =
            ((metrics.alpha_power - m_baseline.alpha_power) / m_baseline.alpha_power) * 100.0;
        double alpha_score = 0.0;

        // Чем ближе к целевому изменению, тем выше оценка
        if (alpha_change_percent >= m_target_metrics.alpha_change) {
            alpha_score = 100.0;
        } else if (alpha_change_percent > 0) {
            alpha_score = (alpha_change_percent / m_target_metrics.alpha_change) * 100.0;
        }

        score += alpha_score * m_target_metrics.alpha_weight;
        total_weight += m_target_metrics.alpha_weight;
    }

    // Проверка изменения концентрации
    if (m_baseline.concentration > 0.0) {
        double conc_change_percent =
            ((metrics.concentration - m_baseline.concentration) / m_baseline.concentration) * 100.0;
        double conc_score = 0.0;

        if (conc_change_percent >= m_target_metrics.concentration_change) {
            conc_score = 100.0;
        } else if (conc_change_percent > 0) {
            conc_score = (conc_change_percent / m_target_metrics.concentration_change) * 100.0;
        }

        score += conc_score * m_target_metrics.concentration_weight;
        total_weight += m_target_metrics.concentration_weight;
    }

    // Проверка снижения бета
    if (m_baseline.beta_power > 0.0) {
        double beta_change_percent =
            ((metrics.beta_power - m_baseline.beta_power) / m_baseline.beta_power) * 100.0;
        double beta_score = 0.0;

        // Для бета нужно снижение, поэтому инвертируем
        if (beta_change_percent <= m_target_metrics.beta_change) {
            beta_score = 100.0;
        } else if (beta_change_percent < 0) {
            beta_score = (std::abs(beta_change_percent) / std::abs(m_target_metrics.beta_change)) * 100.0;
        }

        score += beta_score * m_target_metrics.beta_weight;
        total_weight += m_target_metrics.beta_weight;
    }

    // Проверка уровня релаксации
    if (m_baseline.relaxation > 0.0) {
        double relax_change_percent =
            ((metrics.relaxation - m_baseline.relaxation) / m_baseline.relaxation) * 100.0;
        double relax_score = 0.0;

        if (relax_change_percent >= m_target_metrics.relaxation_change) {
            relax_score = 100.0;
        } else if (relax_change_percent > 0) {
            relax_score = (relax_change_percent / m_target_metrics.relaxation_change) * 100.0;
        }

        score += relax_score * m_target_metrics.relaxation_weight;
        total_weight += m_target_metrics.relaxation_weight;
    }

    // Нормализуем оценку
    if (total_weight > 0.0) {
        score = score / total_weight;
    }

    // Ограничиваем диапазон 0-100
    if (score > 100.0) score = 100.0;
    if (score < 0.0) score = 0.0;

    return score;
}

} // namespace Bronnikov
