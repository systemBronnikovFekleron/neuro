#pragma once
#include "Exercise.h"

namespace Bronnikov {

class VerticalPumping : public Exercise {
public:
    VerticalPumping();
    ~VerticalPumping() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
