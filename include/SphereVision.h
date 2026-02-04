#pragma once
#include "Exercise.h"

namespace Bronnikov {

class SphereVision : public Exercise {
public:
    SphereVision();
    ~SphereVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
