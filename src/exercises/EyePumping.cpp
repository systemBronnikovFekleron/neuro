#include "EyePumping.h"

namespace Bronnikov {

EyePumping::EyePumping()
    : Exercise("Прокачка глаз",
               "Активизация и развитие зрительных функций. Развитие сосудистой, мышечной и энергетической систем глаз.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 12;
    m_duration_minutes = 12;
    initializeInstructions();
    initializeTargetMetrics();
}

void EyePumping::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Учитель создает связь между левым глазом и правым зрительным центром.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Покачивая руками, создайте натянутую 'энергетическую ниточку'.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Аналогично: правый глаз - левый зрительный центр.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Ученик закрывает левый глаз ладонью. Работа с правым глазом.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Учитель вращательными движениями создает контакт с энергопотоком глаза.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Вдох учителя: отводит руку, вбирая энергию. Ученик делает выдох.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Выдох учителя: вталкивает энергию к глазу. Ученик делает вдох.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Аналогично с левым глазом. Затем двумя глазами одновременно.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Снятие напряжения: прикрыть глаза теплыми ладонями.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void EyePumping::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +50.0;
    m_target_metrics.theta_change = +32.0;
    m_target_metrics.concentration_change = +60.0;
    m_target_metrics.relaxation_change = +31.0;
    m_target_metrics.alpha_weight = 1.2;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.2;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 62.0;
    m_target_metrics.theta_min = 37.0;
    m_target_metrics.concentration_min = 70.0;
}

} // namespace Bronnikov
