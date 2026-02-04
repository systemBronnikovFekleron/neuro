#pragma once
#include "Exercise.h"

namespace Bronnikov {

class DistantHealing : public Exercise {
public:
    DistantHealing();
    ~DistantHealing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
