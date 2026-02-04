#include "PastLifeVision.h"

namespace Bronnikov {

PastLifeVision::PastLifeVision()
    : Exercise("Видение прошлых воплощений",
               "Доступ к информации о прошлых жизнях через радарное сканирование.")
{
    m_stage = ExerciseStage::Stage3;
    m_order_in_stage = 23;
    m_duration_minutes = 25;
    initializeInstructions();
    initializeTargetMetrics();
}

void PastLifeVision::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Настройтесь на свою временную линию.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"Направьте внимание в прошлое, за пределы этой жизни.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Увидьте образ прошлого воплощения: место, время, облик.", 300, ExercisePhase::Execution});
    m_instructions.push_back({"Рассмотрите ключевые моменты той жизни.", 300, ExercisePhase::Execution});
    m_instructions.push_back({"Поймите связь прошлого опыта с настоящим.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void PastLifeVision::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +80.0;
    m_target_metrics.theta_change = +65.0;
    m_target_metrics.concentration_change = +75.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 0.9;
    m_target_metrics.alpha_min = 98.0;
    m_target_metrics.theta_min = 50.0;
}

} // namespace Bronnikov
