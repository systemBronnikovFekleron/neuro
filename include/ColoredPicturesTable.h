#pragma once
#include "Exercise.h"

namespace Bronnikov {
class ColoredPicturesTable : public Exercise {
public:
    ColoredPicturesTable();
    ~ColoredPicturesTable() override = default;
protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};
}
