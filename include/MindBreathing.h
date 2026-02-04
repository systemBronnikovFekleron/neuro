#pragma once

#include "Exercise.h"

namespace Bronnikov {

/**
 * @brief Упражнение 4: "Дыхание мыслью"
 *
 * Развитие контроля за внутренними процессами через визуализацию
 * дыхания энергией. Практикующий дышит не воздухом, а энергией.
 *
 * Целевые показатели:
 * - Максимальное повышение альфа-активности (глубокая релаксация)
 * - Снижение бета-активности (отключение ментального шума)
 * - Очень высокая релаксация
 * - Снижение стресса
 * - Минимальная концентрация (состояние потока)
 */
class MindBreathing : public Exercise {
public:
    MindBreathing();
    ~MindBreathing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;

private:
    MetricsSnapshot m_baseline;
};

} // namespace Bronnikov
