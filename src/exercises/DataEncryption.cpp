#include "DataEncryption.h"

namespace Bronnikov {

DataEncryption::DataEncryption()
    : Exercise("Шифрование данных",
               "Защита информации в биокомпьютере через создание ключей доступа.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 14;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void DataEncryption::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Создайте секретное сообщение.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте ключ шифрования (кодовое слово).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Зашифруйте сообщение с помощью ключа.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Сохраните зашифрованное сообщение в памяти.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Расшифруйте сообщение с помощью ключа.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void DataEncryption::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +54.0;
    m_target_metrics.concentration_change = +52.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 78.0;
    m_target_metrics.concentration_min = 82.0;
}

} // namespace Bronnikov
