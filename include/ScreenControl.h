#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 1.4 "Управление экраном зрения вне глаз (ЛБК)"
 *
 * 1-я ступень: Экология духа (порядковый номер 4)
 *
 * Цель: Научиться включать и выключать экран личного биокомпьютера (ЛБК).
 * Базовое упражнение для всей дальнейшей работы с внутренним видением.
 *
 * Целевые метрики:
 * - Alpha 9-11 Hz: высокое (активация зрительной коры без глаз)
 * - Concentration: высокое (фокус на экране)
 * - Beta: умеренное снижение (расслабленная концентрация)
 */
class ScreenControl : public Exercise {
public:
    ScreenControl();
    ~ScreenControl() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;
};

} // namespace Bronnikov
