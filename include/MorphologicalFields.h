#pragma once
#include "Exercise.h"

namespace Bronnikov {

class MorphologicalFields : public Exercise {
public:
    MorphologicalFields();
    ~MorphologicalFields() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
