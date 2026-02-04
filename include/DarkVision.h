#pragma once
#include "Exercise.h"

namespace Bronnikov {

class DarkVision : public Exercise {
public:
    DarkVision();
    ~DarkVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
