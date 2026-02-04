#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 1.1 "Позитивная психологическая настройка"
 *
 * 1-я ступень: Зрение вне глаз (порядковый номер 1)
 *
 * Цель: Приведение себя в рабочее состояние, формирование позитивного настроя.
 * Выполняется перед началом любого упражнения на ЛБК.
 *
 * Целевые метрики:
 * - Alpha (релаксация, внутренний фокус): увеличение на 20-30%
 * - Beta (концентрация): умеренное снижение (расслабление)
 * - Theta (медитативное состояние): умеренное увеличение
 * - Relaxation: высокое (70-90%)
 * - Concentration: умеренное (40-60%)
 */
class PositivePsychology : public Exercise {
public:
    PositivePsychology();
    ~PositivePsychology() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;
};

} // namespace Bronnikov
