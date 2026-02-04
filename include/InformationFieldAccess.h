#pragma once
#include "Exercise.h"

namespace Bronnikov {

class InformationFieldAccess : public Exercise {
public:
    InformationFieldAccess();
    ~InformationFieldAccess() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
