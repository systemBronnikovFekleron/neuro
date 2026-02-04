#pragma once
#include "Exercise.h"

namespace Bronnikov {

class VirtualKeyboard : public Exercise {
public:
    VirtualKeyboard();
    ~VirtualKeyboard() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
