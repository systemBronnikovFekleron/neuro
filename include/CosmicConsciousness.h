#pragma once
#include "Exercise.h"

namespace Bronnikov {

class CosmicConsciousness : public Exercise {
public:
    CosmicConsciousness();
    ~CosmicConsciousness() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
