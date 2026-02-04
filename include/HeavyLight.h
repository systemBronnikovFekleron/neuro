#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 9: "Тяжелое и Легкое"
 *
 * Развитие контроля над восприятием веса и плотности энергии.
 * Практикующий создает контрастные ощущения тяжести и легкости.
 *
 * Целевые показатели:
 * - Умеренная концентрация (переключение состояний)
 * - Баланс альфа и бета активности
 * - Управление психосоматическими ощущениями
 * - Гибкость нервной системы
 */
class HeavyLight : public Exercise {
public:
    HeavyLight();
    ~HeavyLight() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
