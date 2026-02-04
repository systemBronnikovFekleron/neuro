#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 3: "Лучи из пальцев"
 *
 * Развитие точечных фантомных ощущений в кончиках пальцев.
 * Практикующий создает ощущение лучей энергии, исходящих из кончиков пальцев.
 *
 * Целевые показатели:
 * - Повышение бета-активности (высокая концентрация)
 * - Значительное увеличение концентрации
 * - Увеличение фокуса
 * - Минимальная релаксация (акцент на точности)
 */
class FingerRays : public Exercise {
public:
    FingerRays();
    ~FingerRays() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
