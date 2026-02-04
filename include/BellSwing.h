#pragma once
#include "Exercise.h"

namespace Bronnikov {

class BellSwing : public Exercise {
public:
    BellSwing();
    ~BellSwing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
