#pragma once
#include "Exercise.h"

namespace Bronnikov {

class HandSymbolVision : public Exercise {
public:
    HandSymbolVision();
    ~HandSymbolVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
