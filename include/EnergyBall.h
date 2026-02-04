#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 1: "Энергетический шар"
 *
 * Развитие фантомных ощущений между руками.
 * Практикующий создает ощущение энергетического шара между ладонями.
 *
 * Целевые показатели:
 * - Повышение альфа-активности (релаксация с концентрацией)
 * - Увеличение концентрации внимания
 * - Стабильное фокусированное состояние
 * - Снижение бета-активности (меньше ментального шума)
 */
class EnergyBall : public Exercise {
public:
    EnergyBall();
    ~EnergyBall() override = default;

    void start(MetricsCollector& metrics_collector) override;
    void update(const MetricsSnapshot& current_metrics, double elapsed_seconds) override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    // Baseline метрики для сравнения
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
