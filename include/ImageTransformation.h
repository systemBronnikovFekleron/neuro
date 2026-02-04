#pragma once
#include "Exercise.h"

namespace Bronnikov {

class ImageTransformation : public Exercise {
public:
    ImageTransformation();
    ~ImageTransformation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
