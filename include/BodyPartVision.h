#pragma once
#include "Exercise.h"

namespace Bronnikov {

class BodyPartVision : public Exercise {
public:
    BodyPartVision();
    ~BodyPartVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
