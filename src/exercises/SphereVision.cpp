#include "SphereVision.h"

namespace Bronnikov {

SphereVision::SphereVision()
    : Exercise("Видеть вокруг себя (шаровое зрение)",
               "Панорамное альтернативное зрение на 360 градусов через экран ЛБК.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 25;
    m_duration_minutes = 20;
    initializeInstructions();
    initializeTargetMetrics();
}

void SphereVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Расширьте экран вокруг себя.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Видьте одновременно всё, что перед вами.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте боковое зрение (слева и справа).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Добавьте зрение сзади. Полная сфера 360°.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Удерживайте шаровое зрение. Опишите всё вокруг.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК постепенно.", 30, ExercisePhase::Post});
}

void SphereVision::initializeTargetMetrics() {
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
