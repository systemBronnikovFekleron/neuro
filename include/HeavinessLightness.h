#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HeavinessLightness : public Exercise {
public:
    HeavinessLightness();
    ~HeavinessLightness() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
