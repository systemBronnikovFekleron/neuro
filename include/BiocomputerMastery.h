#pragma once
#include "Exercise.h"

namespace Bronnikov {

class BiocomputerMastery : public Exercise {
public:
    BiocomputerMastery();
    ~BiocomputerMastery() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
