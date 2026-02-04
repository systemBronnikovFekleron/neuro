#pragma once
#include "Exercise.h"

namespace Bronnikov {

class PhantomSensations : public Exercise {
public:
    PhantomSensations();
    ~PhantomSensations() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
