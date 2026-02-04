#include "EnergyFieldScanning.h"

namespace Bronnikov {

EnergyFieldScanning::EnergyFieldScanning()
    : Exercise("Сканирование энергетических полей",
               "Радарное восприятие энергетических структур объектов и людей.")
{
    m_stage = ExerciseStage::Stage4;
    m_order_in_stage = 1;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void EnergyFieldScanning::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активируйте радарное восприятие.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Направьте внимание на объект перед собой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Почувствуйте энергетическое поле объекта.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Определите границы, плотность, цвет поля.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Просканируйте несколько объектов подряд.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК постепенно.", 30, ExercisePhase::Post});
}

void EnergyFieldScanning::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +65.0;
    m_target_metrics.theta_change = +48.0;
    m_target_metrics.concentration_change = +60.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 88.0;
    m_target_metrics.concentration_min = 88.0;
}

} // namespace Bronnikov
