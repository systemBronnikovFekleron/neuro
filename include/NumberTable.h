#pragma once
#include "Exercise.h"

namespace Bronnikov {

class NumberTable : public Exercise {
public:
    NumberTable();
    ~NumberTable() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
