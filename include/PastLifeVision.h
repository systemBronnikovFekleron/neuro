#pragma once
#include "Exercise.h"

namespace Bronnikov {

class PastLifeVision : public Exercise {
public:
    PastLifeVision();
    ~PastLifeVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
