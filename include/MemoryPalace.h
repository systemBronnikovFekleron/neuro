#pragma once
#include "Exercise.h"

namespace Bronnikov {

class MemoryPalace : public Exercise {
public:
    MemoryPalace();
    ~MemoryPalace() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
