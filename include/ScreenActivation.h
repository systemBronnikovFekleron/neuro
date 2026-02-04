#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ScreenActivation : public Exercise {
public:
    ScreenActivation();
    ~ScreenActivation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
