#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 7: "Тонкоматериальное тело"
 *
 * Развитие ощущения всего энергетического тела целиком.
 * Практикующий ощущает энергетическую оболочку вокруг физического тела.
 *
 * Целевые показатели:
 * - Высокая альфа-активность (глубокая осознанность)
 * - Умеренная концентрация (целостное восприятие)
 * - Высокая релаксация
 * - Баланс всех показателей
 */
class SubtleBody : public Exercise {
public:
    SubtleBody();
    ~SubtleBody() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
