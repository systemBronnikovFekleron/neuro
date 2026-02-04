#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HorizontalPumping : public Exercise {
public:
    HorizontalPumping();
    ~HorizontalPumping() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
