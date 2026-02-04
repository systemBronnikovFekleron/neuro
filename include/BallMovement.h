#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 2: "Перемещение шара"
 *
 * Развитие управления фантомными ощущениями через движение
 * энергетического шара в пространстве.
 *
 * Целевые показатели:
 * - Сбалансированное повышение альфа и бета активности
 * - Умеренное увеличение концентрации
 * - Умеренное увеличение релаксации
 * - Баланс между сосредоточенностью и расслаблением
 */
class BallMovement : public Exercise {
public:
    BallMovement();
    ~BallMovement() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
