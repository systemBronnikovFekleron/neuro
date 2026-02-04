#pragma once
#include "Exercise.h"

namespace Bronnikov {

class BlindMovement : public Exercise {
public:
    BlindMovement();
    ~BlindMovement() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
