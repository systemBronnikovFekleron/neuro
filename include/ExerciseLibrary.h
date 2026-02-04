#pragma once

#include "Exercise.h"
#include <vector>
#include <memory>
#include <map>

namespace Bronnikov {

/**
 * @brief Библиотека упражнений методики Бронникова
 *
 * Управляет коллекцией упражнений, организованных по ступеням обучения.
 * Предоставляет меню выбора упражнений и доступ к ним.
 */
class ExerciseLibrary {
public:
    ExerciseLibrary();
    ~ExerciseLibrary() = default;

    /**
     * @brief Получить все упражнения (плоский список)
     */
    const std::vector<std::shared_ptr<Exercise>>& getExercises() const;

    /**
     * @brief НОВОЕ: Получить упражнения конкретной ступени
     */
    std::vector<std::shared_ptr<Exercise>> getExercisesByStage(ExerciseStage stage) const;

    /**
     * @brief НОВОЕ: Получить список доступных ступеней
     */
    std::vector<ExerciseStage> getAvailableStages() const;

    /**
     * @brief НОВОЕ: Получить количество упражнений на ступени
     */
    size_t getStageExerciseCount(ExerciseStage stage) const;

    /**
     * @brief Получить упражнение по индексу (0-based) - устаревший метод
     */
    std::shared_ptr<Exercise> getExercise(size_t index) const;

    /**
     * @brief НОВОЕ: Получить упражнение по stage и order
     */
    std::shared_ptr<Exercise> getExerciseByStageOrder(ExerciseStage stage, int order) const;

    /**
     * @brief Получить количество упражнений
     */
    size_t getCount() const { return m_exercises.size(); }

    /**
     * @brief ОБНОВЛЕННОЕ: Показать меню с группировкой по ступеням
     * @return Выбранное упражнение или nullptr если пользователь выбрал выход
     */
    std::shared_ptr<Exercise> showMenu() const;

    /**
     * @brief Получить рекомендуемое упражнение (первое подготовительной ступени)
     */
    std::shared_ptr<Exercise> getRecommended() const;

private:
    // Плоский список (для обратной совместимости)
    std::vector<std::shared_ptr<Exercise>> m_exercises;

    // НОВОЕ: Группировка по ступеням (для быстрого доступа)
    std::map<ExerciseStage, std::vector<std::shared_ptr<Exercise>>> m_exercises_by_stage;

    /**
     * @brief Зарегистрировать все упражнения в библиотеке
     */
    void registerExercises();

    /**
     * @brief НОВОЕ: Построение индекса по ступеням
     */
    void buildStageIndex();
};

} // namespace Bronnikov
