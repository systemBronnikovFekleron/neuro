#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ImageProcessing : public Exercise {
public:
    ImageProcessing();
    ~ImageProcessing() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
