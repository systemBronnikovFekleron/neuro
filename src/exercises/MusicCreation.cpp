#include "MusicCreation.h"

namespace Bronnikov {

MusicCreation::MusicCreation()
    : Exercise("Создание музыки",
               "Генерация мелодий и звуковых композиций через биокомпьютер.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 11;
    m_duration_minutes = 14;
    initializeInstructions();
    initializeTargetMetrics();
}

void MusicCreation::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Услышьте простую ноту (до).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте последовательность нот (гамма).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Сочините простую мелодию из 5-7 нот.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте ритм и гармонию.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Прослушайте созданную композицию.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void MusicCreation::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +56.0;
    m_target_metrics.theta_change = +38.0;
    m_target_metrics.concentration_change = +48.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 78.0;
}

} // namespace Bronnikov
