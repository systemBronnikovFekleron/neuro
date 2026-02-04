#include "DepthVision.h"

namespace Bronnikov {

DepthVision::DepthVision()
    : Exercise("Видеть мозгом в глубину",
               "Развитие способности видеть объекты сквозь препятствия и на глубине.")
{
    m_stage = ExerciseStage::Stage2;
    m_order_in_stage = 20;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void DepthVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Направьте внимание на стену перед собой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Проникните взглядом ЛБК сквозь стену. Что за ней?", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите детали: цвета, формы, движение.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Попробуйте увидеть объекты в соседней комнате.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void DepthVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +55.0;
    m_target_metrics.theta_change = +35.0;
    m_target_metrics.concentration_change = +50.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.8;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 78.0;
}

} // namespace Bronnikov
