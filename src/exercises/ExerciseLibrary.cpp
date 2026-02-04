#include "ExerciseLibrary.h"
#include "EnergyBall.h"
#include "BallMovement.h"
#include "FingerRays.h"
#include "MindBreathing.h"
#include "FastWind.h"
#include "EyeRays.h"
#include "SubtleBody.h"
#include "HeavenEarth.h"
#include "HeavyLight.h"
#include "VolumePerception.h"

#include <iostream>
#include <limits>
#include <algorithm>

namespace Bronnikov {

ExerciseLibrary::ExerciseLibrary() {
    registerExercises();
    buildStageIndex();  // НОВОЕ: строим индекс после регистрации
}

void ExerciseLibrary::registerExercises() {
    // Регистрируем все 10 упражнений методики Бронникова (1-я ступень)
    m_exercises.push_back(std::make_shared<EnergyBall>());
    m_exercises.push_back(std::make_shared<BallMovement>());
    m_exercises.push_back(std::make_shared<FingerRays>());
    m_exercises.push_back(std::make_shared<MindBreathing>());
    m_exercises.push_back(std::make_shared<FastWind>());
    m_exercises.push_back(std::make_shared<EyeRays>());
    m_exercises.push_back(std::make_shared<SubtleBody>());
    m_exercises.push_back(std::make_shared<HeavenEarth>());
    m_exercises.push_back(std::make_shared<HeavyLight>());
    m_exercises.push_back(std::make_shared<VolumePerception>());
}

const std::vector<std::shared_ptr<Exercise>>& ExerciseLibrary::getExercises() const {
    return m_exercises;
}

std::shared_ptr<Exercise> ExerciseLibrary::getExercise(size_t index) const {
    if (index >= m_exercises.size()) {
        return nullptr;
    }
    return m_exercises[index];
}

std::shared_ptr<Exercise> ExerciseLibrary::showMenu() const {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           Методика Бронникова - Выбор упражнения             ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;

    auto available_stages = getAvailableStages();

    int global_index = 1;  // Сквозная нумерация для ввода
    std::map<int, std::shared_ptr<Exercise>> index_to_exercise;

    for (ExerciseStage stage : available_stages) {
        auto exercises = getExercisesByStage(stage);

        if (exercises.empty()) continue;

        // Заголовок ступени
        std::cout << "\n┌─ " << getStageDisplayName(stage)
                  << " (" << exercises.size() << " упр.) ────" << std::endl;

        // Упражнения
        for (const auto& ex : exercises) {
            std::cout << "  [" << global_index << "] "
                      << ex->getStageId() << " " << ex->getName()
                      << " (" << ex->getDuration() << " мин)" << std::endl;

            if (global_index == 1) {
                std::cout << "      ⭐ Рекомендовано для начинающих" << std::endl;
            }

            index_to_exercise[global_index] = ex;
            global_index++;
        }

        std::cout << "└────────────────────────────────────────────────────────────────" << std::endl;
    }

    std::cout << "\n  [0] Выход из программы" << std::endl;
    std::cout << "\n═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "Ваш выбор (0-" << (global_index - 1) << "): ";

    int choice;
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "⚠️  Некорректный ввод. Введите число от 0 до "
                  << (global_index - 1) << ": ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return nullptr;
    }

    auto it = index_to_exercise.find(choice);
    if (it != index_to_exercise.end()) {
        return it->second;
    }

    std::cout << "⚠️  Неверный выбор! Попробуйте снова." << std::endl;
    return showMenu();
}

std::shared_ptr<Exercise> ExerciseLibrary::getRecommended() const {
    // Для новичков рекомендуем первое упражнение - "Энергетический шар"
    return m_exercises.empty() ? nullptr : m_exercises[0];
}

// НОВЫЕ МЕТОДЫ для поддержки ступеней

void ExerciseLibrary::buildStageIndex() {
    m_exercises_by_stage.clear();

    for (const auto& exercise : m_exercises) {
        ExerciseStage stage = exercise->getStage();
        m_exercises_by_stage[stage].push_back(exercise);
    }

    // Сортировка по order_in_stage внутри каждой ступени
    for (auto& [stage, exercises] : m_exercises_by_stage) {
        std::sort(exercises.begin(), exercises.end(),
            [](const auto& a, const auto& b) {
                return a->getOrderInStage() < b->getOrderInStage();
            });
    }
}

std::vector<std::shared_ptr<Exercise>> ExerciseLibrary::getExercisesByStage(ExerciseStage stage) const {
    auto it = m_exercises_by_stage.find(stage);
    if (it != m_exercises_by_stage.end()) {
        return it->second;
    }
    return {};
}

std::vector<ExerciseStage> ExerciseLibrary::getAvailableStages() const {
    std::vector<ExerciseStage> stages;
    for (const auto& [stage, exercises] : m_exercises_by_stage) {
        if (!exercises.empty()) {
            stages.push_back(stage);
        }
    }
    // Сортировка по порядку ступеней
    std::sort(stages.begin(), stages.end());
    return stages;
}

size_t ExerciseLibrary::getStageExerciseCount(ExerciseStage stage) const {
    auto it = m_exercises_by_stage.find(stage);
    return (it != m_exercises_by_stage.end()) ? it->second.size() : 0;
}

std::shared_ptr<Exercise> ExerciseLibrary::getExerciseByStageOrder(
    ExerciseStage stage, int order) const {

    auto exercises = getExercisesByStage(stage);
    for (const auto& ex : exercises) {
        if (ex->getOrderInStage() == order) {
            return ex;
        }
    }
    return nullptr;
}

} // namespace Bronnikov
