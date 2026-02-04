#include "HeadPumping.h"

namespace Bronnikov {

HeadPumping::HeadPumping()
    : Exercise("Прокачка головы",
               "Активизация и гармонизация энергоструктуры головного мозга. Зарядка для мозга.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 6;
    m_duration_minutes = 14;
    initializeInstructions();
    initializeTargetMetrics();
}

void HeadPumping::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Смотрите на черную точку на стене (70-80 см).", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Позиция 1: Лоб-затылок. Прокачка на вдохе и выдохе 10-20 раз.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Позиция 2: Темя-затылок (прокачка мозжечка) 10-20 раз.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Позиция 3: Диагональные прокачки (левый затылок - правый лоб).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Позиция 4: Затылочно-теменная часть параллельно 10-20 раз.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Позиция 5: Макушка. Вертикальная прокачка вниз-вверх через стопы.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"'Шлем' и гармонизация поля над головой для выравнивания.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void HeadPumping::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +48.0;
    m_target_metrics.theta_change = +30.0;
    m_target_metrics.concentration_change = +58.0;
    m_target_metrics.relaxation_change = +32.0;
    m_target_metrics.alpha_weight = 1.2;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.1;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 60.0;
    m_target_metrics.theta_min = 35.0;
    m_target_metrics.concentration_min = 68.0;
}

} // namespace Bronnikov
