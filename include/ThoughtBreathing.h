#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ThoughtBreathing : public Exercise {
public:
    ThoughtBreathing();
    ~ThoughtBreathing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
