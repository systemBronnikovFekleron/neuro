#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 8: "Небо и Земля"
 *
 * Развитие вертикального энергетического потока.
 * Практикующий создает связь между земной и космической энергией.
 *
 * Целевые показатели:
 * - Высокая альфа-активность (медитативное состояние)
 * - Умеренная концентрация (канал энергии)
 * - Высокая релаксация
 * - Повышение тета-активности
 */
class HeavenEarth : public Exercise {
public:
    HeavenEarth();
    ~HeavenEarth() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
