#pragma once
#include "Exercise.h"

namespace Bronnikov {

class DepthVision : public Exercise {
public:
    DepthVision();
    ~DepthVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
