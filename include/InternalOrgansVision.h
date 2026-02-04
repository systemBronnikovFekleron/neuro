#pragma once
#include "Exercise.h"

namespace Bronnikov {

class InternalOrgansVision : public Exercise {
public:
    InternalOrgansVision();
    ~InternalOrgansVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
