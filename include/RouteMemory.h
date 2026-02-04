#pragma once
#include "Exercise.h"

namespace Bronnikov {

class RouteMemory : public Exercise {
public:
    RouteMemory();
    ~RouteMemory() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
