#pragma once
#include "Exercise.h"

namespace Bronnikov {

class TextRotation : public Exercise {
public:
    TextRotation();
    ~TextRotation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
