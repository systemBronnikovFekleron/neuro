#include "HorizontalPumping.h"

namespace Bronnikov {

HorizontalPumping::HorizontalPumping()
    : Exercise("Горизонтальная прокачка",
               "Открытие жизненных врат. Активизация энергии на клеточном уровне.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 13;
    m_duration_minutes = 18;
    initializeInstructions();
    initializeTargetMetrics();
}

void HorizontalPumping::initializeInstructions() {
    m_instructions.push_back({"Поза лежа на животе, руки вдоль тела ладонями вверх. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Глаза открыты. Учитель активизирует центр в области копчика.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель поднимает энергию 'фонтанчиком' вверх, возвращается. 3-5 раз.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Растягивание энергии в сторону ног на 5-7 метров. Взбивание 'перины' 3-5 раз.", 150, ExercisePhase::Execution});
    m_instructions.push_back({"Растягивание по 4 направлениям: ноги, левый бок, голова, правый бок.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Прокачка по позвоночнику вверх к голове.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик переворачивается на спину. Активизация центра ниже пупка 'фонтанчиком'.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Растягивание энергии на спине по 4 направлениям: голова, левый, ноги, правый.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Гармонизация поля приглаживающими движениями по кругу.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Протаскивание энергетического шарика 'волной' через все тело к ногам.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void HorizontalPumping::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +49.0;
    m_target_metrics.theta_change = +33.0;
    m_target_metrics.concentration_change = +58.0;
    m_target_metrics.relaxation_change = +42.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.relaxation_weight = 1.2;
    m_target_metrics.alpha_min = 61.0;
    m_target_metrics.theta_min = 38.0;
    m_target_metrics.concentration_min = 68.0;
}

} // namespace Bronnikov
