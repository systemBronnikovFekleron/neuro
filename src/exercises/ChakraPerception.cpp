#include "ChakraPerception.h"

namespace Bronnikov {

ChakraPerception::ChakraPerception()
    : Exercise("Восприятие чакр",
               "Сканирование и анализ состояния энергетических центров (чакр).")
{
    m_stage = ExerciseStage::Stage4;
    m_order_in_stage = 3;
    m_duration_minutes = 20;
    initializeInstructions();
    initializeTargetMetrics();
}

void ChakraPerception::initializeInstructions() {
    m_instructions.push_back({"Поза 'кучера', повязка. Базовые метрики.", 30, ExercisePhase::Baseline});
    m_instructions.push_back({"Включите ЛБК. Активируйте восприятие чакр.", 90, ExercisePhase::Execution});
    m_instructions.push_back({"Просканируйте первую чакру (муладхара). Цвет, размер.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Поднимитесь выше: вторая чакра (свадхистана).", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Продолжите: третья (манипура), четвёртая (анахата).", 180, ExercisePhase::Execution});
    m_instructions.push_back({"Завершите сканирование: пятая, шестая, седьмая чакры.", 240, ExercisePhase::Execution});
    m_instructions.push_back({"Оцените баланс всей системы чакр.", 120, ExercisePhase::Execution});
    m_instructions.push_back({"Выключите ЛБК.", 30, ExercisePhase::Post});
}

void ChakraPerception::initializeTargetMetrics() {
    m_target_metrics.alpha_change = +70.0;
    m_target_metrics.theta_change = +52.0;
    m_target_metrics.concentration_change = +65.0;
    m_target_metrics.alpha_weight = 1.0;
    m_target_metrics.theta_weight = 0.9;
    m_target_metrics.concentration_weight = 1.0;
    m_target_metrics.alpha_min = 92.0;
    m_target_metrics.concentration_min = 90.0;
}

} // namespace Bronnikov
