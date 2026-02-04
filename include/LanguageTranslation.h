#pragma once
#include "Exercise.h"

namespace Bronnikov {

class LanguageTranslation : public Exercise {
public:
    LanguageTranslation();
    ~LanguageTranslation() override = default;

protected:
    void initializeInstructions() override;
    void initializeTargetMetrics() override;
};

} // namespace Bronnikov
