#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HandTextVision : public Exercise {
public:
    HandTextVision();
    ~HandTextVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
