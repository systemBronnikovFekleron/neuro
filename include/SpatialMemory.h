#pragma once
#include "Exercise.h"

namespace Bronnikov {

class SpatialMemory : public Exercise {
public:
    SpatialMemory();
    ~SpatialMemory() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
