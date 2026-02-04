#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HeadPumping : public Exercise {
public:
    HeadPumping();
    ~HeadPumping() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
