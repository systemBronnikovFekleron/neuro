#pragma once
#include "Exercise.h"

namespace Bronnikov {

class InformationSearch : public Exercise {
public:
    InformationSearch();
    ~InformationSearch() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
