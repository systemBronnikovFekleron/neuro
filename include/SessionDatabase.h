#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include "MetricsCollector.h"

namespace Bronnikov {

/**
 * @brief Запись сессии упражнения в базе данных
 */
struct SessionRecord {
    int id = 0;
    std::string user_id;
    std::string exercise_name;
    std::string timestamp;  // ISO 8601 format (YYYY-MM-DD HH:MM:SS)
    int duration_seconds = 0;
    double success_rate = 0.0;

    // НОВЫЕ ПОЛЯ для поддержки ступеней
    int exercise_stage = 0;          // Ступень упражнения (0=Подг, 1=1-я, 2=2-я, 3=3-я)
    int exercise_order_in_stage = 0; // Порядковый номер в ступени

    // Средние метрики за сессию (active фаза)
    double avg_alpha = 0.0;
    double avg_beta = 0.0;
    double avg_theta = 0.0;
    double avg_concentration = 0.0;
    double avg_relaxation = 0.0;
    double avg_fatigue = 0.0;
    double avg_focus = 0.0;
    double avg_stress = 0.0;
    double avg_heart_rate = 0.0;

    // НОВЫЕ ПОЛЯ: Baseline метрики (до упражнения)
    double baseline_alpha = 0.0;
    double baseline_beta = 0.0;
    double baseline_theta = 0.0;
    double baseline_concentration = 0.0;
    double baseline_relaxation = 0.0;
    double baseline_fatigue = 0.0;
    double baseline_heart_rate = 0.0;

    // НОВЫЕ ПОЛЯ: Post метрики (после упражнения)
    double post_alpha = 0.0;
    double post_beta = 0.0;
    double post_theta = 0.0;
    double post_concentration = 0.0;
    double post_relaxation = 0.0;
    double post_fatigue = 0.0;
    double post_heart_rate = 0.0;

    // НОВЫЕ ПОЛЯ: Изменения между фазами
    double alpha_change = 0.0;
    double beta_change = 0.0;
    double theta_change = 0.0;
    double concentration_change = 0.0;
    double relaxation_change = 0.0;
    double fatigue_change = 0.0;

    // НОВЫЕ ПОЛЯ: Эффективность
    double overall_effectiveness = 0.0;
    bool target_achieved = false;

    std::string csv_file_path;  // Путь к CSV файлу с детальными данными
};

/**
 * @brief Статистика пользователя
 */
struct UserStats {
    int total_sessions = 0;
    double total_hours = 0.0;
    double avg_success_rate = 0.0;
    std::string first_session_date;
    std::string last_session_date;
};

/**
 * @brief НОВОЕ: Статистика по ступени обучения
 */
struct StageStats {
    int stage = 0;                    // Номер ступени (0=Подг, 1=1-я, 2=2-я, 3=3-я)
    int total_exercises_in_stage = 0; // Всего упражнений на ступени
    int completed_exercises = 0;      // Освоено (success >= 70%)
    int total_sessions = 0;           // Всего сессий на ступени
    double avg_success_rate = 0.0;    // Средний успех
    double completion_percentage = 0.0; // Процент освоения ступени
};

/**
 * @brief Уровень практики пользователя
 */
enum class PracticeLevel {
    Beginner = 0,      // Новичок: 1-2 минуты
    Intermediate = 1,  // Практикующий: 2-5 минут
    Expert = 2         // Специалист: до 10 минут
};

/**
 * @brief Получить название уровня практики
 */
inline std::string getPracticeLevelName(PracticeLevel level) {
    switch (level) {
        case PracticeLevel::Beginner:     return "Новичок";
        case PracticeLevel::Intermediate: return "Практикующий";
        case PracticeLevel::Expert:       return "Специалист";
        default:                          return "Неизвестно";
    }
}

/**
 * @brief Получить диапазон времени для уровня практики
 */
inline std::pair<int, int> getPracticeLevelDurationRange(PracticeLevel level) {
    switch (level) {
        case PracticeLevel::Beginner:     return {1, 2};   // 1-2 минуты
        case PracticeLevel::Intermediate: return {2, 5};   // 2-5 минут
        case PracticeLevel::Expert:       return {5, 10};  // до 10 минут
        default:                          return {5, 10};
    }
}

/**
 * @brief Профиль пользователя
 */
struct UserProfileData {
    std::string user_id;
    std::string name;
    std::string created_at;
    std::string last_calibration_date;

    // НОВОЕ: Уровень практики
    PracticeLevel practice_level = PracticeLevel::Beginner;

    double iaf = 0.0;   // Individual Alpha Frequency
    double iapf = 0.0;  // Individual Alpha Peak Frequency

    // Baseline метрики
    double baseline_alpha = 0.0;
    double baseline_beta = 0.0;
    double baseline_theta = 0.0;
    double baseline_concentration = 0.0;
    double baseline_relaxation = 0.0;
    double baseline_heart_rate = 0.0;
};

/**
 * @brief База данных для хранения сессий упражнений
 *
 * Использует SQLite для персистентного хранения истории
 * всех выполненных упражнений.
 */
class SessionDatabase {
public:
    /**
     * @brief Конструктор
     * @param db_path Путь к файлу базы данных SQLite
     */
    SessionDatabase(const std::string& db_path = "data/bronnikov_sessions.db");
    ~SessionDatabase();

    // Запрет копирования
    SessionDatabase(const SessionDatabase&) = delete;
    SessionDatabase& operator=(const SessionDatabase&) = delete;

    /**
     * @brief Открыть базу данных и создать таблицы если не существуют
     * @return true если успешно
     */
    bool open();

    /**
     * @brief Закрыть базу данных
     */
    void close();

    /**
     * @brief Проверка открыта ли БД
     */
    bool isOpen() const { return m_db != nullptr; }

    /**
     * @brief Сохранить запись сессии
     * @param record Запись сессии
     * @return true если успешно сохранено
     */
    bool saveSession(const SessionRecord& record);

    /**
     * @brief Получить историю сессий пользователя
     * @param user_id ID пользователя (default = "default")
     * @param limit Максимальное количество записей (0 = все)
     * @return Вектор записей, отсортированных по дате (от новых к старым)
     */
    std::vector<SessionRecord> getSessions(const std::string& user_id = "default",
                                           int limit = 100) const;

    /**
     * @brief Получить сессии по конкретному упражнению
     * @param exercise_name Название упражнения
     * @param user_id ID пользователя
     * @return Вектор записей по данному упражнению
     */
    std::vector<SessionRecord> getSessionsByExercise(const std::string& exercise_name,
                                                      const std::string& user_id = "default") const;

    /**
     * @brief Получить последнюю сессию по упражнению
     * @param exercise_name Название упражнения
     * @param user_id ID пользователя
     * @return Запись или пустая запись если не найдено
     */
    SessionRecord getLastSession(const std::string& exercise_name,
                                  const std::string& user_id = "default") const;

    /**
     * @brief Получить общую статистику пользователя
     * @param user_id ID пользователя
     * @return Структура со статистикой
     */
    UserStats getUserStats(const std::string& user_id = "default") const;

    /**
     * @brief НОВОЕ: Получить сессии по конкретной ступени
     * @param stage Номер ступени (0=Подг, 1=1-я, 2=2-я, 3=3-я)
     * @param user_id ID пользователя
     * @return Вектор записей по данной ступени
     */
    std::vector<SessionRecord> getSessionsByStage(int stage,
                                                   const std::string& user_id = "default") const;

    /**
     * @brief НОВОЕ: Получить статистику по конкретной ступени
     * @param stage Номер ступени
     * @param user_id ID пользователя
     * @return Структура со статистикой ступени
     */
    StageStats getStageStats(int stage, const std::string& user_id = "default") const;

    /**
     * @brief НОВОЕ: Получить прогресс по всем ступеням
     * @param user_id ID пользователя
     * @return Вектор статистики по всем ступеням
     */
    std::vector<StageStats> getAllStagesProgress(const std::string& user_id = "default") const;

    /**
     * @brief Удалить старые сессии (старше указанного количества дней)
     * @param days_to_keep Количество дней для хранения
     * @return Количество удаленных записей
     */
    int cleanupOldSessions(int days_to_keep = 365);

    /**
     * @brief Загрузить профиль пользователя
     * @param user_id ID пользователя
     * @return Структура с данными профиля (пустая если не найдено)
     */
    UserProfileData loadUserProfile(const std::string& user_id = "default");

    /**
     * @brief Сохранить профиль пользователя (INSERT или UPDATE)
     * @param profile Данные профиля
     * @return true если успешно
     */
    bool saveUserProfile(const UserProfileData& profile);

    /**
     * @brief Получить список всех пользователей
     * @return Вектор с ID всех пользователей
     */
    std::vector<std::string> getAllUserIds();

private:
    std::string m_db_path;
    sqlite3* m_db = nullptr;

    /**
     * @brief Создать таблицы в БД
     */
    bool createTables();

    /**
     * @brief Выполнить SQL запрос без результата
     */
    bool executeSQL(const std::string& sql) const;

    /**
     * @brief Получить текущий timestamp в формате ISO 8601
     */
    static std::string getCurrentTimestamp();

    /**
     * @brief НОВОЕ: Миграция старых сессий (добавление полей stage)
     */
    bool migrateOldSessions();
};

} // namespace Bronnikov
