#pragma once
#include "Exercise.h"

namespace Bronnikov {

class AnatomicalAtlas : public Exercise {
public:
    AnatomicalAtlas();
    ~AnatomicalAtlas() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
