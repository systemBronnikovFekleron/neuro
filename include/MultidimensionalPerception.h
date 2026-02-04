#pragma once
#include "Exercise.h"

namespace Bronnikov {

class MultidimensionalPerception : public Exercise {
public:
    MultidimensionalPerception();
    ~MultidimensionalPerception() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
