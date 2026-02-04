#include "SmallDragon.h"

namespace Bronnikov {

SmallDragon::SmallDragon()
    : Exercise("Малый Дракон",
               "Техника прокачки костных каналов. Развитие внутрикостных и межорганных энергетических каналов.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 9;
    m_duration_minutes = 11;
    initializeInstructions();
    initializeTargetMetrics();
}

void SmallDragon::initializeInstructions() {
    m_instructions.push_back({"Поза стоя: ноги на ширине плеч, слегка согнуты. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Руки согнуты в локтях, ладони на уровне плеч вверх.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Дыхание через рот. Вдох: язык к нёбу, губы почти сомкнуты.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Вдох: всасывайте энергию через 3 верхних точки (ладони + лицо).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Перемещайте энергию до солнечного сплетения. Задержка дыхания.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выдох: выводите энергию через 3 нижних точки (промежность + ступни).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Смена направления: вдох через нижние точки, выдох через верхние.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Меняйте ритм дыхания и скорость передвижения энергии.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Заканчивайте на восходящих потоках (выдох через верхние точки).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void SmallDragon::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +45.0;
    m_target_metrics.theta_change = +29.0;
    m_target_metrics.concentration_change = +55.0;
    m_target_metrics.relaxation_change = +35.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.1;
    m_target_metrics.relaxation_weight = 0.9;
    m_target_metrics.alpha_min = 58.0;
    m_target_metrics.theta_min = 34.0;
    m_target_metrics.concentration_min = 65.0;
}

} // namespace Bronnikov
