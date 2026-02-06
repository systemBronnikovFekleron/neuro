#include "UserProfile.h"
#include "SessionDatabase.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Bronnikov {

UserProfile::UserProfile(const std::string& user_id)
    : m_user_id(user_id)
    , m_name(user_id)
{
    // Устанавливаем дату создания
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
    m_created_at = ss.str();
}

bool UserProfile::load(SessionDatabase& db) {
    if (!db.isOpen()) {
        return false;
    }

    // Загружаем профиль из БД
    UserProfileData data = db.loadUserProfile(m_user_id);

    if (data.user_id.empty()) {
        // Профиль не найден - это новый пользователь
        m_exists_in_db = false;
        return false;
    }

    // Заполняем данные из БД
    m_name = data.name;
    m_created_at = data.created_at;
    m_last_calibration_date = data.last_calibration_date;
    m_iaf = data.iaf;
    m_iapf = data.iapf;

    // Заполняем baseline метрики
    m_baseline.alpha_power = data.baseline_alpha;
    m_baseline.beta_power = data.baseline_beta;
    m_baseline.theta_power = data.baseline_theta;
    m_baseline.concentration = data.baseline_concentration;
    m_baseline.relaxation = data.baseline_relaxation;
    m_baseline.heart_rate = data.baseline_heart_rate;

    m_exists_in_db = true;
    return true;
}

bool UserProfile::save(SessionDatabase& db) {
    if (!db.isOpen()) {
        return false;
    }

    // Подготавливаем данные для сохранения
    UserProfileData data;
    data.user_id = m_user_id;
    data.name = m_name;
    data.created_at = m_created_at;
    data.last_calibration_date = m_last_calibration_date;
    data.iaf = m_iaf;
    data.iapf = m_iapf;

    // Копируем baseline метрики
    data.baseline_alpha = m_baseline.alpha_power;
    data.baseline_beta = m_baseline.beta_power;
    data.baseline_theta = m_baseline.theta_power;
    data.baseline_concentration = m_baseline.concentration;
    data.baseline_relaxation = m_baseline.relaxation;
    data.baseline_heart_rate = m_baseline.heart_rate;

    // Сохраняем в БД (INSERT или UPDATE)
    bool success = db.saveUserProfile(data);
    if (success) {
        m_exists_in_db = true;
    }

    return success;
}

void UserProfile::updateBaseline(const MetricsSnapshot& metrics) {
    m_baseline = metrics;
}

void UserProfile::updateCalibration(double iaf, double iapf) {
    m_iaf = iaf;
    m_iapf = iapf;

    // Обновляем дату последней калибровки
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
    m_last_calibration_date = ss.str();
}

bool UserProfile::needsCalibration() const {
    if (m_last_calibration_date.empty()) {
        return true;  // Калибровка никогда не проводилась
    }

    // Парсим дату последней калибровки
    std::tm tm = {};
    std::istringstream ss(m_last_calibration_date);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        return true;  // Ошибка парсинга - лучше откалибровать
    }

    auto last_calibration = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto now = std::chrono::system_clock::now();

    // Проверяем, прошло ли более 7 дней
    auto duration = std::chrono::duration_cast<std::chrono::hours>(now - last_calibration);
    return duration.count() > (7 * 24);  // 7 дней в часах
}

} // namespace Bronnikov
