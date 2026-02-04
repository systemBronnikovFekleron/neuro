#pragma once
#include "Exercise.h"

namespace Bronnikov {

class DataEncryption : public Exercise {
public:
    DataEncryption();
    ~DataEncryption() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
