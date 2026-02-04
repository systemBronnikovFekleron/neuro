#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 10: "Восприятие объема"
 *
 * Развитие трехмерного энергетического восприятия.
 * Практикующий воспринимает объемные энергетические структуры.
 *
 * Целевые показатели:
 * - Высокая концентрация (3D визуализация)
 * - Умеренная альфа-активность (пространственное восприятие)
 * - Высокая бета-активность (сложная обработка)
 * - Баланс концентрации и релаксации
 */
class VolumePerception : public Exercise {
public:
    VolumePerception();
    ~VolumePerception() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
