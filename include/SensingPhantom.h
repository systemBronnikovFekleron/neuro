#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 1.3 "Ощущающий фантом"
 *
 * 1-я ступень: Экология духа (порядковый номер 3)
 *
 * Цель: Развитие виртуального тела (ощущающего фантома).
 * Ученик учится расширять виртуальное тело, заполняя пространство.
 *
 * Целевые метрики:
 * - Alpha: высокое (глубокая концентрация на внутренних ощущениях)
 * - Theta: повышение (медитативное состояние)
 * - Concentration: умеренное (удержание образа фантома)
 * - Relaxation: высокое (расслабленное состояние)
 */
class SensingPhantom : public Exercise {
public:
    SensingPhantom();
    ~SensingPhantom() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;
};

} // namespace Bronnikov
