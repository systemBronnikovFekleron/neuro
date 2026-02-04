#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ScreenColoring : public Exercise {
public:
    ScreenColoring();
    ~ScreenColoring() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
    double checkTargetState(const MetricsSnapshot& metrics) const override;
};

} // namespace Bronnikov
