#pragma once
#include "Exercise.h"

namespace Bronnikov {

class AbstractModels : public Exercise {
public:
    AbstractModels();
    ~AbstractModels() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
