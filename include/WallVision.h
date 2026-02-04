#pragma once
#include "Exercise.h"

namespace Bronnikov {

class WallVision : public Exercise {
public:
    WallVision();
    ~WallVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
