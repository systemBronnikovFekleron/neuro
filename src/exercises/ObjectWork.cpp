#include "ObjectWork.h"

namespace Bronnikov {

ObjectWork::ObjectWork()
    : Exercise("Работа через предметы",
               "Опосредованная энергоинформационная работа через предметы. Расширение диапазона сверхсознания.")
{
    m_stage = ExerciseStage::Stage1;
    m_order_in_stage = 15;
    m_duration_minutes = 11;
    initializeInstructions();
    initializeTargetMetrics();
}

void ObjectWork::initializeInstructions() {
    m_instructions.push_back({"Поза сидя или стоя. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активизируйте ощущение энергии в руках.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Поднимите руку над предметом (стол, стул) на 50-70 см.", 60, ExercisePhase::Execution});
    m_instructions.push_back({"Медленно опускайте руку, ощущая сжатие энергетического поля.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Коснитесь поверхности. Медленно поднимайте, чувствуя сопротивление отрыву.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Отключите внимание - ощущения исчезают. Включите - появляются снова.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Создайте контакт с предметом на расстоянии 2-3 метра.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Пошлите поток энергии из ладони, ощутите отраженный сигнал.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"'Виртуальной рукой' прикоснитесь к стенам помещения, определите расстояние.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК медленно.", 30, ExercisePhase::Post});
}

void ObjectWork::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +51.0;
    m_target_metrics.theta_change = +34.0;
    m_target_metrics.concentration_change = +61.0;
    m_target_metrics.relaxation_change = +32.0;
    m_target_metrics.alpha_weight = 1.1;
    m_target_metrics.theta_weight = 1.0;
    m_target_metrics.concentration_weight = 1.2;
    m_target_metrics.relaxation_weight = 0.8;
    m_target_metrics.alpha_min = 63.0;
    m_target_metrics.theta_min = 39.0;
    m_target_metrics.concentration_min = 71.0;
}

} // namespace Bronnikov
