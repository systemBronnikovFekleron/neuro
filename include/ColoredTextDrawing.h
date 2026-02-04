#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ColoredTextDrawing : public Exercise {
public:
    ColoredTextDrawing();
    ~ColoredTextDrawing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
