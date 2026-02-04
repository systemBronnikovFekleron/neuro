#pragma once
#include "Exercise.h"

namespace Bronnikov {

class FuturePrediction : public Exercise {
public:
    FuturePrediction();
    ~FuturePrediction() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
