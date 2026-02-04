#include "BiocomputerMastery.h"

namespace Bronnikov {

BiocomputerMastery::BiocomputerMastery()
    : Exercise("Мастерство владения биокомпьютером",
               "Комплексное упражнение, объединяющее все навыки работы с ЛБК.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 15;
    m_duration_minutes = 20;
    initializeInstructions();
    initializeTargetMetrics();
}

void BiocomputerMastery::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активируйте все модули биокомпьютера.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Загрузите текст, изображение и числа одновременно.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Обработайте данные: переведите, вычислите, трансформируйте.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте новую композицию из обработанных данных.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Сохраните результат в дворце памяти.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Продемонстрируйте мастерство: быстрый доступ к любым данным.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК постепенно.", 30, ExercisePhase::Post});
}

void BiocomputerMastery::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +70.0;
    m_target_metrics.theta_change = +50.0;
    m_target_metrics.concentration_change = +65.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 90.0;
    m_target_metrics.concentration_min = 90.0;
}

} // namespace Bronnikov
