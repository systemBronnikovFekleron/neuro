#pragma once

#include <string>
#include "MetricsCollector.h"

namespace Bronnikov {

// Forward declaration
class SessionDatabase;

/**
 * @brief Профиль пользователя с baseline метриками и калибровкой
 *
 * Сохраняет baseline метрики между сессиями для точной оценки прогресса.
 */
class UserProfile {
public:
    UserProfile(const std::string& user_id = "default");
    ~UserProfile() = default;

    /**
     * @brief Загрузить профиль из БД
     * @param db База данных
     * @return true если профиль найден
     */
    bool load(SessionDatabase& db);

    /**
     * @brief Сохранить профиль в БД
     * @param db База данных
     * @return true если успешно сохранено
     */
    bool save(SessionDatabase& db);

    /**
     * @brief Обновить baseline метрики
     * @param metrics Снимок метрик для сохранения как baseline
     */
    void updateBaseline(const MetricsSnapshot& metrics);

    /**
     * @brief Обновить данные калибровки (IAF, IAPF)
     * @param iaf Individual Alpha Frequency
     * @param iapf Individual Alpha Peak Frequency
     */
    void updateCalibration(double iaf, double iapf = 0.0);

    /**
     * @brief Проверка: нужна ли калибровка?
     * @return true если последняя калибровка была более 7 дней назад или никогда не было
     */
    bool needsCalibration() const;

    // Геттеры
    const std::string& getUserId() const { return m_user_id; }
    const std::string& getName() const { return m_name; }
    const MetricsSnapshot& getBaseline() const { return m_baseline; }
    double getIAF() const { return m_iaf; }
    double getIAPF() const { return m_iapf; }
    const std::string& getLastCalibrationDate() const { return m_last_calibration_date; }
    const std::string& getCreatedAt() const { return m_created_at; }

    // Сеттеры
    void setName(const std::string& name) { m_name = name; }

private:
    std::string m_user_id;
    std::string m_name;
    std::string m_created_at;
    std::string m_last_calibration_date;

    double m_iaf = 0.0;   // Individual Alpha Frequency
    double m_iapf = 0.0;  // Individual Alpha Peak Frequency

    MetricsSnapshot m_baseline;  // Baseline метрики пользователя

    bool m_exists_in_db = false;  // Флаг существования в БД
};

} // namespace Bronnikov
