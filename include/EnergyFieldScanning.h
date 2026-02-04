#pragma once
#include "Exercise.h"

namespace Bronnikov {

class EnergyFieldScanning : public Exercise {
public:
    EnergyFieldScanning();
    ~EnergyFieldScanning() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
