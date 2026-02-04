#pragma once
#include "Exercise.h"

namespace Bronnikov {

class EyePumping : public Exercise {
public:
    EyePumping();
    ~EyePumping() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
