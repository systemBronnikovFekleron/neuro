#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ColoredCellsTable : public Exercise {
public:
    ColoredCellsTable();
    ~ColoredCellsTable() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
