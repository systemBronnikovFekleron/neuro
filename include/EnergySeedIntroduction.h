#pragma once
#include "Exercise.h"

namespace Bronnikov {

class EnergySeedIntroduction : public Exercise {
public:
    EnergySeedIntroduction();
    ~EnergySeedIntroduction() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
