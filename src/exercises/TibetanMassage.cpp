#include "TibetanMassage.h"

namespace Bronnikov {

TibetanMassage::TibetanMassage()
    : Exercise("Сухой Тибетский Массаж",
               "Техника 'продувки' и всасывания жизненной силы через энергетические каналы.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 8;
    m_duration_minutes = 13;
    initializeInstructions();
    initializeTargetMetrics();
}

void TibetanMassage::initializeInstructions() {
    m_instructions.push_back({"Поза стоя, ноги на ширине плеч. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Энергично растирайте руки до ощутимого тепла.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Растирайте: руки (внешняя и внутренняя стороны) до тепла.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Растирайте: ноги (внешняя и внутренняя стороны).", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Растирайте: область копчика и живота.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Растирайте: голову сзади (затылок) и спереди (лицо).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Вдох через рот (язык у нёба): вытаскивайте энергию по внешней стороне руки.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выдох через рот: вдавливайте энергию вовнутрь по внутренней стороне.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Повторите для ног, головы, копчик-живот. По 5-10 раз каждое.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void TibetanMassage::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +44.0;
    m_target_metrics.theta_change = +27.0;
    m_target_metrics.concentration_change = +53.0;
    m_target_metrics.relaxation_change = +40.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.1;
    m_target_metrics.alpha_min = 57.0;
    m_target_metrics.theta_min = 32.0;
    m_target_metrics.concentration_min = 63.0;
}

} // namespace Bronnikov
