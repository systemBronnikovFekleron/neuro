#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 6: "Лучи из глаз"
 *
 * Развитие визуальных фантомных ощущений.
 * Практикующий создает ощущение лучей энергии, исходящих из глаз.
 *
 * Целевые показатели:
 * - Высокая концентрация (визуальный контроль)
 * - Повышение бета-активности
 * - Увеличение фокуса
 * - Умеренная релаксация
 */
class EyeRays : public Exercise {
public:
    EyeRays();
    ~EyeRays() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
