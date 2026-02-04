#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ProblemSolving : public Exercise {
public:
    ProblemSolving();
    ~ProblemSolving() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
