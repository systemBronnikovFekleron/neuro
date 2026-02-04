#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 1.2 "Энергетические руки"
 *
 * 1-я ступень: Экология духа (порядковый номер 2)
 *
 * Цель: Развитие чувствительности виртуальных (энергетических) рук.
 * Ученик учится управлять энергетическими руками независимо от физических.
 *
 * Целевые метрики:
 * - Alpha: увеличение (внутренний фокус на энергетических ощущениях)
 * - Concentration: умеренное повышение (удержание внимания)
 * - Relaxation: высокое (расслабленная концентрация)
 */
class EnergyHandsStage1 : public Exercise {
public:
    EnergyHandsStage1();
    ~EnergyHandsStage1() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;
};

} // namespace Bronnikov
