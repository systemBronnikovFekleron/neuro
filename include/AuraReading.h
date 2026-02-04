#pragma once
#include "Exercise.h"

namespace Bronnikov {

class AuraReading : public Exercise {
public:
    AuraReading();
    ~AuraReading() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
