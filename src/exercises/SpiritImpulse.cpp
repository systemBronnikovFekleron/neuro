#include "SpiritImpulse.h"

namespace Bronnikov {

SpiritImpulse::SpiritImpulse()
    : Exercise("Импульс Святого Духа",
               "Начальная техника Импульса Святого Духа (Энергетический всплеск).")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 7;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void SpiritImpulse::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Учитель формирует шарик в копчике ученика.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Этап 1 (глаза открыты): Учитель перемещает энергию от копчика вдоль позвоночника.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Когда энергия достигает головы - выплеснуть через глаза вдаль 10-30 сек.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Этап 2 (глаза закрыты): При достижении головы - резко открыть глаза, выплеск!", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Этап 3: Ученик сам накачивает шарик в копчике и перемещает вверх.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Этап 4: Самостоятельное выполнение от начала до конца.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Доведите выполнение до автоматизма за 2-3 секунды. Наращивайте силу всплеска.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void SpiritImpulse::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +46.0;
    m_target_metrics.theta_change = +28.0;
    m_target_metrics.concentration_change = +56.0;
    m_target_metrics.relaxation_change = +30.0;
    m_target_metrics.alpha_weight = 1.1;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.2;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 58.0;
    m_target_metrics.theta_min = 33.0;
    m_target_metrics.concentration_min = 66.0;
}

} // namespace Bronnikov
