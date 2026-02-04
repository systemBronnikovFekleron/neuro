#pragma once
#include "Exercise.h"

namespace Bronnikov {

class CreativeVisualization : public Exercise {
public:
    CreativeVisualization();
    ~CreativeVisualization() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
