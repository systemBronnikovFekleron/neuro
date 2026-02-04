#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 5: "Быстрый ветер"
 *
 * Развитие динамического управления энергетическими потоками.
 * Практикующий создает быстрые движения энергии вокруг тела.
 *
 * Целевые показатели:
 * - Умеренное повышение бета-активности (динамичная концентрация)
 * - Повышение альфа-активности (контролируемая энергия)
 * - Баланс концентрации и подвижности
 * - Увеличение фокуса
 */
class FastWind : public Exercise {
public:
    FastWind();
    ~FastWind() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
