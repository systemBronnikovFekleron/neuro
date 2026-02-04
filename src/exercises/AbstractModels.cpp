#include "AbstractModels.h"

namespace Bronnikov {

AbstractModels::AbstractModels()
    : Exercise("Работа по абстрактно-логическим моделям",
               "Оздоровление и развитие через собственные схематические изображения органов и систем.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 17;
    m_duration_minutes = 13;
    initializeInstructions();
    initializeTargetMetrics();
}

void AbstractModels::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Листы бумаги и ручка рядом. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Схематично нарисуйте нужный орган/систему/процесс.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Можно нарисовать: зуб, голову, ногу, нижний центр, температурную шкалу.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Можно изобразить морфологические поля (5 уровней), график объективизации идеи.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте энергетический контакт с человеком или собой.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Перенесите связь на свой рисунок (абстрактно-логическую модель).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Работайте с фантомными ощущениями по вашему рисунку 3-5 минут.", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Модель работает как 'пульт управления' для реального объекта.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"По окончании разорвите контакт. Выключите ЛБК медленно.", 60, ExercisePhase::Post});
}

void AbstractModels::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +53.0;
    m_target_metrics.theta_change = +36.0;
    m_target_metrics.concentration_change = +63.0;
    m_target_metrics.relaxation_change = +34.0;
    m_target_metrics.alpha_weight = 1.1;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.2;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 65.0;
    m_target_metrics.theta_min = 41.0;
    m_target_metrics.concentration_min = 73.0;
}

} // namespace Bronnikov
