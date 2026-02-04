#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ObjectVision : public Exercise {
public:
    ObjectVision();
    ~ObjectVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
