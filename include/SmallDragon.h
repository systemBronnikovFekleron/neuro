#pragma once
#include "Exercise.h"

namespace Bronnikov {

class SmallDragon : public Exercise {
public:
    SmallDragon();
    ~SmallDragon() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
