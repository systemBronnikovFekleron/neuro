#pragma once
#include "Exercise.h"

namespace Bronnikov {

class TimeManagement : public Exercise {
public:
    TimeManagement();
    ~TimeManagement() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
