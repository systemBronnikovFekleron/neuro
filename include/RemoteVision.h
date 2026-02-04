#pragma once
#include "Exercise.h"

namespace Bronnikov {

class RemoteVision : public Exercise {
public:
    RemoteVision();
    ~RemoteVision() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
