#include "BellSwing.h"

namespace Bronnikov {

BellSwing::BellSwing()
    : Exercise("Нижний и Верхний Колокол",
               "Раскачка и управление полевыми структурами. Развитие двух энергетических центров.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 10;
    m_duration_minutes = 13;
    initializeInstructions();
    initializeTargetMetrics();
}

void BellSwing::initializeInstructions() {
    m_instructions.push_back({"Поза стоя: ноги на ширине плеч, колени слегка согнуты. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Руки обращены ладонями к нижнему центру, слегка разведены.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Нижний колокол: вершина в центре головы, широкая часть внизу.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Захватите руками плотность поля. Перемещайте конус вперед-назад, влево-вправо.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Вращайте колокол по и против часовой стрелки. Ощущения в голове: сжатие.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Остановите физические руки. 'Энергетические руки' продолжают движение.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Верхний колокол: широкая часть вверх, крепление в нижнем центре.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Руки подняты вверх, согнуты в локтях. Перемещайте колокол во все стороны.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Работа 'энергетическими руками' с верхним колоколом.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void BellSwing::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +47.0;
    m_target_metrics.theta_change = +30.0;
    m_target_metrics.concentration_change = +57.0;
    m_target_metrics.relaxation_change = +33.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.1;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 59.0;
    m_target_metrics.theta_min = 35.0;
    m_target_metrics.concentration_min = 67.0;
}

} // namespace Bronnikov
