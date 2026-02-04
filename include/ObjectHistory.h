#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ObjectHistory : public Exercise {
public:
    ObjectHistory();
    ~ObjectHistory() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
