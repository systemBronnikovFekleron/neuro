#pragma once
#include "Exercise.h"

namespace Bronnikov {

class TibetanMassage : public Exercise {
public:
    TibetanMassage();
    ~TibetanMassage() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
