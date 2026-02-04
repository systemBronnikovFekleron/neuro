#pragma once
#include "Exercise.h"

namespace Bronnikov {

class MusicCreation : public Exercise {
public:
    MusicCreation();
    ~MusicCreation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
