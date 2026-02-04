#pragma once
#include "Exercise.h"

namespace Bronnikov {

class InformationLoading : public Exercise {
public:
    InformationLoading();
    ~InformationLoading() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
