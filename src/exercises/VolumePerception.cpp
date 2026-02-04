#include "VolumePerception.h"

namespace Bronnikov {

VolumePerception::VolumePerception()
    : Exercise("Восприятие объема",
               "Развитие трехмерного энергетического восприятия. "
               "Практикующий воспринимает объемные энергетические структуры "
               "и пространственные отношения.")
{
    // НОВОЕ: маркируем как подготовительную ступень
    m_stage = ExerciseStage::Preparatory;
    m_order_in_stage = 10;  // Десятое упражнение подготовительной ступени

    initializeInstructions();
    initializeTargetMetrics();
}

void VolumePerception::initializeInstructions() {
    m_instructions = {
        // Preparation Phase
        {"Сядьте удобно с прямой спиной", 10, ExercisePhase::Preparation},
        {"Закройте глаза", 10, ExercisePhase::Preparation},
        {"Сделайте несколько глубоких вдохов", 20, ExercisePhase::Preparation},
        {"Сосредоточьтесь на пространстве вокруг вас", 15, ExercisePhase::Preparation},

        // Execution Phase
        {"Представьте перед собой куб светлой энергии (20x20x20 см)", 30, ExercisePhase::Execution},
        {"Почувствуйте все грани куба одновременно", 30, ExercisePhase::Execution},
        {"Ощутите внутренний объем куба", 25, ExercisePhase::Execution},
        {"Медленно вращайте куб в пространстве", 30, ExercisePhase::Execution},
        {"Измените форму: куб превращается в сферу", 30, ExercisePhase::Execution},
        {"Почувствуйте поверхность и объем сферы", 25, ExercisePhase::Execution},
        {"Создайте вторую сферу рядом с первой", 30, ExercisePhase::Execution},
        {"Ощутите обе сферы одновременно", 30, ExercisePhase::Execution},
        {"Соедините сферы энергетической нитью", 25, ExercisePhase::Execution},
        {"Создайте трехмерную структуру из нескольких объектов", 35, ExercisePhase::Execution},
        {"Воспринимайте всю структуру как единое целое", 35, ExercisePhase::Execution},
        {"Расширьте восприятие на все пространство вокруг вас", 30, ExercisePhase::Execution},

        // Completion Phase
        {"Постепенно растворите все объекты", 20, ExercisePhase::Completion},
        {"Верните внимание к своему телу", 15, ExercisePhase::Completion},
        {"Сделайте несколько глубоких вдохов", 15, ExercisePhase::Completion},
        {"Медленно откройте глаза", 10, ExercisePhase::Completion},
    };
}

void VolumePerception::initializeTargetMetrics() {
    // Целевое состояние: высокая пространственная обработка
    // Упражнение требует активной работы визуально-пространственных областей

    m_target_metrics.alpha_change = 15.0;           // +15% (спокойная концентрация)
    m_target_metrics.beta_change = 20.0;            // +20% (сложная обработка)
    m_target_metrics.theta_change = 5.0;            // +5% (творческая визуализация)

    m_target_metrics.concentration_change = 28.0;   // +28% (высокая концентрация)
    m_target_metrics.relaxation_change = 12.0;      // +12% (контролируемая)

    // Веса для scoring - акцент на концентрации и бета-активности
    m_target_metrics.alpha_weight = 0.8;
    m_target_metrics.beta_weight = 1.4;             // Бета критична
    m_target_metrics.theta_weight = 0.5;
    m_target_metrics.concentration_weight = 1.5;    // Концентрация критична
    m_target_metrics.relaxation_weight = 0.6;
}

} // namespace Bronnikov
