#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ChakraPerception : public Exercise {
public:
    ChakraPerception();
    ~ChakraPerception() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
