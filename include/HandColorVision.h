#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HandColorVision : public Exercise {
public:
    HandColorVision();
    ~HandColorVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
