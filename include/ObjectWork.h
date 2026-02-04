#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ObjectWork : public Exercise {
public:
    ObjectWork();
    ~ObjectWork() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
