#pragma once
#include "Exercise.h"

namespace Bronnikov {

class BiocomputerActivation : public Exercise {
public:
    BiocomputerActivation();
    ~BiocomputerActivation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
