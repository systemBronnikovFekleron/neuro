#include "VerticalPumping.h"

namespace Bronnikov {

VerticalPumping::VerticalPumping()
    : Exercise("Вертикальная прокачка",
               "Активизация и развитие жизненных сил. Основы Астрального карате.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 3;
    m_duration_minutes = 15;
    initializeInstructions();
    initializeTargetMetrics();
}

void VerticalPumping::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Учитель и ученик устанавливают энергетический контакт.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик отходит на 5-7 метров, сохраняя контакт через ладони.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Работа с внешней формой: взбивание поля снизу вверх, уплотнение по бокам.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Поворот на 180°. Прокачка со спины.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Работа с внутренней формой: прокачка вдоль позвоночника.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Прокачка по бокам и через плечи.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"'Шлем' и гармонизация поля над головой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void VerticalPumping::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +42.0;
    m_target_metrics.theta_change = +25.0;
    m_target_metrics.concentration_change = +52.0;
    m_target_metrics.relaxation_change = +38.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.0;
    m_target_metrics.alpha_min = 55.0;
    m_target_metrics.theta_min = 30.0;
    m_target_metrics.concentration_min = 62.0;
}

} // namespace Bronnikov
