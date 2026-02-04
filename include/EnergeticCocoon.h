#pragma once
#include "Exercise.h"

namespace Bronnikov {

class EnergeticCocoon : public Exercise {
public:
    EnergeticCocoon();
    ~EnergeticCocoon() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
