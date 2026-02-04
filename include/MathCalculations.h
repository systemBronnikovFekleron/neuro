#pragma once
#include "Exercise.h"

namespace Bronnikov {

class MathCalculations : public Exercise {
public:
    MathCalculations();
    ~MathCalculations() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
