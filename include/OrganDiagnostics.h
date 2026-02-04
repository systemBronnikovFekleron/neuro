#pragma once
#include "Exercise.h"

namespace Bronnikov {

class OrganDiagnostics : public Exercise {
public:
    OrganDiagnostics();
    ~OrganDiagnostics() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
