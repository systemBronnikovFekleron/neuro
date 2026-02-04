#include "SessionDatabase.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <sys/stat.h>

namespace Bronnikov {

SessionDatabase::SessionDatabase(const std::string& db_path)
    : m_db_path(db_path)
{
}

SessionDatabase::~SessionDatabase() {
    close();
}

bool SessionDatabase::open() {
    if (m_db) {
        return true;  // Уже открыта
    }

    // Создаем директорию data/ если не существует
    std::string dir = m_db_path.substr(0, m_db_path.find_last_of('/'));
    if (!dir.empty()) {
        mkdir(dir.c_str(), 0755);  // Игнорируем ошибку если уже существует
    }

    // Открываем БД (создастся автоматически если не существует)
    int rc = sqlite3_open(m_db_path.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        std::cerr << "[SessionDatabase] ✗ Не удалось открыть БД: "
                  << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    std::cout << "[SessionDatabase] ✓ БД открыта: " << m_db_path << std::endl;

    // Создаем таблицы если не существуют
    if (!createTables()) {
        std::cerr << "[SessionDatabase] ✗ Не удалось создать таблицы" << std::endl;
        close();
        return false;
    }

    // НОВОЕ: миграция старых данных
    migrateOldSessions();

    return true;
}

void SessionDatabase::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SessionDatabase::createTables() {
    const char* create_sessions_table = R"(
        CREATE TABLE IF NOT EXISTS sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT NOT NULL,
            exercise_name TEXT NOT NULL,
            timestamp TEXT NOT NULL,
            duration_seconds INTEGER NOT NULL,
            success_rate REAL NOT NULL,
            exercise_stage INTEGER DEFAULT 0,
            exercise_order_in_stage INTEGER DEFAULT 1,
            avg_alpha REAL,
            avg_beta REAL,
            avg_theta REAL,
            avg_concentration REAL,
            avg_relaxation REAL,
            avg_fatigue REAL,
            avg_focus REAL,
            avg_stress REAL,
            avg_heart_rate REAL,
            baseline_alpha REAL,
            baseline_beta REAL,
            baseline_theta REAL,
            baseline_concentration REAL,
            baseline_relaxation REAL,
            baseline_fatigue REAL,
            baseline_heart_rate REAL,
            post_alpha REAL,
            post_beta REAL,
            post_theta REAL,
            post_concentration REAL,
            post_relaxation REAL,
            post_fatigue REAL,
            post_heart_rate REAL,
            alpha_change REAL,
            beta_change REAL,
            theta_change REAL,
            concentration_change REAL,
            relaxation_change REAL,
            fatigue_change REAL,
            overall_effectiveness REAL,
            target_achieved INTEGER DEFAULT 0,
            csv_file_path TEXT
        );
    )";

    const char* create_user_index = R"(
        CREATE INDEX IF NOT EXISTS idx_user_timestamp
        ON sessions(user_id, timestamp DESC);
    )";

    const char* create_exercise_index = R"(
        CREATE INDEX IF NOT EXISTS idx_exercise
        ON sessions(exercise_name);
    )";

    // НОВЫЕ индексы для быстрого поиска по ступеням
    const char* create_stage_index = R"(
        CREATE INDEX IF NOT EXISTS idx_stage
        ON sessions(exercise_stage, user_id);
    )";

    const char* create_stage_order_index = R"(
        CREATE INDEX IF NOT EXISTS idx_stage_order
        ON sessions(exercise_stage, exercise_order_in_stage, user_id);
    )";

    const char* create_users_table = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT UNIQUE NOT NULL,
            name TEXT,
            created_at TEXT NOT NULL,
            last_calibration_date TEXT,
            practice_level INTEGER DEFAULT 0,
            iaf REAL,
            iapf REAL,
            baseline_alpha REAL,
            baseline_beta REAL,
            baseline_theta REAL,
            baseline_concentration REAL,
            baseline_relaxation REAL,
            baseline_heart_rate REAL
        );
    )";

    if (!executeSQL(create_sessions_table)) return false;
    if (!executeSQL(create_user_index)) return false;
    if (!executeSQL(create_exercise_index)) return false;
    if (!executeSQL(create_stage_index)) return false;        // НОВОЕ
    if (!executeSQL(create_stage_order_index)) return false;  // НОВОЕ
    if (!executeSQL(create_users_table)) return false;

    return true;
}

bool SessionDatabase::executeSQL(const std::string& sql) const {
    if (!m_db) return false;

    char* error_msg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &error_msg);

    if (rc != SQLITE_OK) {
        std::cerr << "[SessionDatabase] SQL error: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        return false;
    }

    return true;
}

bool SessionDatabase::saveSession(const SessionRecord& record) {
    if (!m_db) {
        std::cerr << "[SessionDatabase] ✗ БД не открыта" << std::endl;
        return false;
    }

    const char* insert_sql = R"(
        INSERT INTO sessions (
            user_id, exercise_name, timestamp, duration_seconds, success_rate,
            exercise_stage, exercise_order_in_stage,
            avg_alpha, avg_beta, avg_theta,
            avg_concentration, avg_relaxation, avg_fatigue,
            avg_focus, avg_stress, avg_heart_rate,
            baseline_alpha, baseline_beta, baseline_theta,
            baseline_concentration, baseline_relaxation, baseline_fatigue, baseline_heart_rate,
            post_alpha, post_beta, post_theta,
            post_concentration, post_relaxation, post_fatigue, post_heart_rate,
            alpha_change, beta_change, theta_change,
            concentration_change, relaxation_change, fatigue_change,
            overall_effectiveness, target_achieved,
            csv_file_path
        ) VALUES (?, ?, ?, ?, ?,  ?, ?,  ?, ?, ?,  ?, ?, ?,  ?, ?, ?,
                  ?, ?, ?,  ?, ?, ?, ?,  ?, ?, ?,  ?, ?, ?, ?,
                  ?, ?, ?,  ?, ?, ?,  ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, insert_sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[SessionDatabase] ✗ Prepare error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    // Bind параметры
    std::string timestamp = record.timestamp.empty() ? getCurrentTimestamp() : record.timestamp;

    sqlite3_bind_text(stmt, 1, record.user_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, record.exercise_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, timestamp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, record.duration_seconds);
    sqlite3_bind_double(stmt, 5, record.success_rate);
    sqlite3_bind_int(stmt, 6, record.exercise_stage);
    sqlite3_bind_int(stmt, 7, record.exercise_order_in_stage);

    // Active метрики (8-16)
    sqlite3_bind_double(stmt, 8, record.avg_alpha);
    sqlite3_bind_double(stmt, 9, record.avg_beta);
    sqlite3_bind_double(stmt, 10, record.avg_theta);
    sqlite3_bind_double(stmt, 11, record.avg_concentration);
    sqlite3_bind_double(stmt, 12, record.avg_relaxation);
    sqlite3_bind_double(stmt, 13, record.avg_fatigue);
    sqlite3_bind_double(stmt, 14, record.avg_focus);
    sqlite3_bind_double(stmt, 15, record.avg_stress);
    sqlite3_bind_double(stmt, 16, record.avg_heart_rate);

    // Baseline метрики (17-23)
    sqlite3_bind_double(stmt, 17, record.baseline_alpha);
    sqlite3_bind_double(stmt, 18, record.baseline_beta);
    sqlite3_bind_double(stmt, 19, record.baseline_theta);
    sqlite3_bind_double(stmt, 20, record.baseline_concentration);
    sqlite3_bind_double(stmt, 21, record.baseline_relaxation);
    sqlite3_bind_double(stmt, 22, record.baseline_fatigue);
    sqlite3_bind_double(stmt, 23, record.baseline_heart_rate);

    // Post метрики (24-30)
    sqlite3_bind_double(stmt, 24, record.post_alpha);
    sqlite3_bind_double(stmt, 25, record.post_beta);
    sqlite3_bind_double(stmt, 26, record.post_theta);
    sqlite3_bind_double(stmt, 27, record.post_concentration);
    sqlite3_bind_double(stmt, 28, record.post_relaxation);
    sqlite3_bind_double(stmt, 29, record.post_fatigue);
    sqlite3_bind_double(stmt, 30, record.post_heart_rate);

    // Изменения (31-36)
    sqlite3_bind_double(stmt, 31, record.alpha_change);
    sqlite3_bind_double(stmt, 32, record.beta_change);
    sqlite3_bind_double(stmt, 33, record.theta_change);
    sqlite3_bind_double(stmt, 34, record.concentration_change);
    sqlite3_bind_double(stmt, 35, record.relaxation_change);
    sqlite3_bind_double(stmt, 36, record.fatigue_change);

    // Эффективность (37-38)
    sqlite3_bind_double(stmt, 37, record.overall_effectiveness);
    sqlite3_bind_int(stmt, 38, record.target_achieved ? 1 : 0);

    // CSV путь (39)
    sqlite3_bind_text(stmt, 39, record.csv_file_path.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "[SessionDatabase] ✗ Insert error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    std::cout << "[SessionDatabase] ✓ Сессия сохранена: "
              << record.exercise_name << " (Success: " << record.success_rate << "%)" << std::endl;

    return true;
}

std::vector<SessionRecord> SessionDatabase::getSessions(const std::string& user_id, int limit) const {
    std::vector<SessionRecord> sessions;

    if (!m_db) return sessions;

    std::string sql = R"(
        SELECT id, user_id, exercise_name, timestamp, duration_seconds, success_rate,
               avg_alpha, avg_beta, avg_theta,
               avg_concentration, avg_relaxation, avg_fatigue,
               avg_focus, avg_stress, avg_heart_rate,
               csv_file_path
        FROM sessions
        WHERE user_id = ?
        ORDER BY timestamp DESC
    )";

    if (limit > 0) {
        sql += " LIMIT " + std::to_string(limit);
    }

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[SessionDatabase] Query error: " << sqlite3_errmsg(m_db) << std::endl;
        return sessions;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        SessionRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.exercise_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        record.duration_seconds = sqlite3_column_int(stmt, 4);
        record.success_rate = sqlite3_column_double(stmt, 5);
        record.avg_alpha = sqlite3_column_double(stmt, 6);
        record.avg_beta = sqlite3_column_double(stmt, 7);
        record.avg_theta = sqlite3_column_double(stmt, 8);
        record.avg_concentration = sqlite3_column_double(stmt, 9);
        record.avg_relaxation = sqlite3_column_double(stmt, 10);
        record.avg_fatigue = sqlite3_column_double(stmt, 11);
        record.avg_focus = sqlite3_column_double(stmt, 12);
        record.avg_stress = sqlite3_column_double(stmt, 13);
        record.avg_heart_rate = sqlite3_column_double(stmt, 14);

        const char* csv_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        if (csv_path) {
            record.csv_file_path = csv_path;
        }

        sessions.push_back(record);
    }

    sqlite3_finalize(stmt);
    return sessions;
}

std::vector<SessionRecord> SessionDatabase::getSessionsByExercise(const std::string& exercise_name,
                                                                    const std::string& user_id) const {
    std::vector<SessionRecord> sessions;

    if (!m_db) return sessions;

    std::string sql = R"(
        SELECT id, user_id, exercise_name, timestamp, duration_seconds, success_rate,
               avg_alpha, avg_beta, avg_theta,
               avg_concentration, avg_relaxation, avg_fatigue,
               avg_focus, avg_stress, avg_heart_rate,
               csv_file_path
        FROM sessions
        WHERE user_id = ? AND exercise_name = ?
        ORDER BY timestamp DESC
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return sessions;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, exercise_name.c_str(), -1, SQLITE_TRANSIENT);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        SessionRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.exercise_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        record.duration_seconds = sqlite3_column_int(stmt, 4);
        record.success_rate = sqlite3_column_double(stmt, 5);
        record.avg_alpha = sqlite3_column_double(stmt, 6);
        record.avg_beta = sqlite3_column_double(stmt, 7);
        record.avg_theta = sqlite3_column_double(stmt, 8);
        record.avg_concentration = sqlite3_column_double(stmt, 9);
        record.avg_relaxation = sqlite3_column_double(stmt, 10);
        record.avg_fatigue = sqlite3_column_double(stmt, 11);
        record.avg_focus = sqlite3_column_double(stmt, 12);
        record.avg_stress = sqlite3_column_double(stmt, 13);
        record.avg_heart_rate = sqlite3_column_double(stmt, 14);

        const char* csv_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        if (csv_path) {
            record.csv_file_path = csv_path;
        }

        sessions.push_back(record);
    }

    sqlite3_finalize(stmt);
    return sessions;
}

SessionRecord SessionDatabase::getLastSession(const std::string& exercise_name,
                                               const std::string& user_id) const {
    SessionRecord record;

    if (!m_db) return record;

    std::string sql = R"(
        SELECT id, user_id, exercise_name, timestamp, duration_seconds, success_rate,
               avg_alpha, avg_beta, avg_theta,
               avg_concentration, avg_relaxation, avg_fatigue,
               avg_focus, avg_stress, avg_heart_rate,
               csv_file_path
        FROM sessions
        WHERE user_id = ? AND exercise_name = ?
        ORDER BY timestamp DESC
        LIMIT 1
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return record;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, exercise_name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        record.id = sqlite3_column_int(stmt, 0);
        record.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.exercise_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        record.duration_seconds = sqlite3_column_int(stmt, 4);
        record.success_rate = sqlite3_column_double(stmt, 5);
        record.avg_alpha = sqlite3_column_double(stmt, 6);
        record.avg_beta = sqlite3_column_double(stmt, 7);
        record.avg_theta = sqlite3_column_double(stmt, 8);
        record.avg_concentration = sqlite3_column_double(stmt, 9);
        record.avg_relaxation = sqlite3_column_double(stmt, 10);
        record.avg_fatigue = sqlite3_column_double(stmt, 11);
        record.avg_focus = sqlite3_column_double(stmt, 12);
        record.avg_stress = sqlite3_column_double(stmt, 13);
        record.avg_heart_rate = sqlite3_column_double(stmt, 14);

        const char* csv_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        if (csv_path) {
            record.csv_file_path = csv_path;
        }
    }

    sqlite3_finalize(stmt);
    return record;
}

UserStats SessionDatabase::getUserStats(const std::string& user_id) const {
    UserStats stats;

    if (!m_db) return stats;

    const char* sql = R"(
        SELECT
            COUNT(*) as total_sessions,
            SUM(duration_seconds) / 3600.0 as total_hours,
            AVG(success_rate) as avg_success_rate,
            MIN(timestamp) as first_session,
            MAX(timestamp) as last_session
        FROM sessions
        WHERE user_id = ?
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return stats;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stats.total_sessions = sqlite3_column_int(stmt, 0);
        stats.total_hours = sqlite3_column_double(stmt, 1);
        stats.avg_success_rate = sqlite3_column_double(stmt, 2);

        const char* first = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* last = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        if (first) stats.first_session_date = first;
        if (last) stats.last_session_date = last;
    }

    sqlite3_finalize(stmt);
    return stats;
}

int SessionDatabase::cleanupOldSessions(int days_to_keep) {
    if (!m_db) return 0;

    std::string sql = R"(
        DELETE FROM sessions
        WHERE datetime(timestamp) < datetime('now', '-' || ? || ' days')
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return 0;
    }

    sqlite3_bind_int(stmt, 1, days_to_keep);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    int deleted = sqlite3_changes(m_db);
    if (deleted > 0) {
        std::cout << "[SessionDatabase] Удалено старых сессий: " << deleted << std::endl;
    }

    return deleted;
}

std::string SessionDatabase::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    localtime_r(&time_t_now, &tm_now);

    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_now);
    return std::string(buffer);
}

UserProfileData SessionDatabase::loadUserProfile(const std::string& user_id) {
    UserProfileData profile;
    profile.user_id = user_id;

    if (!m_db) return profile;

    const char* sql = R"(
        SELECT user_id, name, created_at, last_calibration_date,
               iaf, iapf,
               baseline_alpha, baseline_beta, baseline_theta,
               baseline_concentration, baseline_relaxation, baseline_heart_rate
        FROM users
        WHERE user_id = ?
        LIMIT 1
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return profile;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        profile.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (name) profile.name = name;

        const char* created = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (created) profile.created_at = created;

        const char* last_cal = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        if (last_cal) profile.last_calibration_date = last_cal;

        profile.iaf = sqlite3_column_double(stmt, 4);
        profile.iapf = sqlite3_column_double(stmt, 5);
        profile.baseline_alpha = sqlite3_column_double(stmt, 6);
        profile.baseline_beta = sqlite3_column_double(stmt, 7);
        profile.baseline_theta = sqlite3_column_double(stmt, 8);
        profile.baseline_concentration = sqlite3_column_double(stmt, 9);
        profile.baseline_relaxation = sqlite3_column_double(stmt, 10);
        profile.baseline_heart_rate = sqlite3_column_double(stmt, 11);

        std::cout << "[SessionDatabase] ✓ Профиль загружен: " << user_id << std::endl;
    }

    sqlite3_finalize(stmt);
    return profile;
}

bool SessionDatabase::saveUserProfile(const UserProfileData& profile) {
    if (!m_db) {
        std::cerr << "[SessionDatabase] ✗ БД не открыта" << std::endl;
        return false;
    }

    // INSERT OR REPLACE для upsert
    const char* sql = R"(
        INSERT OR REPLACE INTO users (
            user_id, name, created_at, last_calibration_date,
            practice_level,
            iaf, iapf,
            baseline_alpha, baseline_beta, baseline_theta,
            baseline_concentration, baseline_relaxation, baseline_heart_rate
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "[SessionDatabase] ✗ Prepare error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    std::string created_at = profile.created_at.empty() ? getCurrentTimestamp() : profile.created_at;

    sqlite3_bind_text(stmt, 1, profile.user_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, profile.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, created_at.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, profile.last_calibration_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, static_cast<int>(profile.practice_level));  // НОВОЕ
    sqlite3_bind_double(stmt, 6, profile.iaf);
    sqlite3_bind_double(stmt, 7, profile.iapf);
    sqlite3_bind_double(stmt, 8, profile.baseline_alpha);
    sqlite3_bind_double(stmt, 9, profile.baseline_beta);
    sqlite3_bind_double(stmt, 10, profile.baseline_theta);
    sqlite3_bind_double(stmt, 11, profile.baseline_concentration);
    sqlite3_bind_double(stmt, 12, profile.baseline_relaxation);
    sqlite3_bind_double(stmt, 13, profile.baseline_heart_rate);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "[SessionDatabase] ✗ Insert profile error: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    std::cout << "[SessionDatabase] ✓ Профиль сохранен: " << profile.user_id << std::endl;
    return true;
}

// НОВЫЕ МЕТОДЫ для поддержки ступеней

bool SessionDatabase::migrateOldSessions() {
    if (!m_db) return false;

    // Проверяем, существуют ли уже поля exercise_stage
    const char* check_sql = "PRAGMA table_info(sessions)";
    sqlite3_stmt* stmt = nullptr;

    bool has_stage_field = false;
    if (sqlite3_prepare_v2(m_db, check_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* col_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (col_name && std::string(col_name) == "exercise_stage") {
                has_stage_field = true;
                break;
            }
        }
        sqlite3_finalize(stmt);
    }

    // Если поля уже существуют, ничего не делаем
    if (has_stage_field) {
        return true;
    }

    // Добавляем новые поля
    std::cout << "[SessionDatabase] 🔄 Миграция: добавление полей exercise_stage..." << std::endl;

    const char* add_stage = "ALTER TABLE sessions ADD COLUMN exercise_stage INTEGER DEFAULT 0";
    const char* add_order = "ALTER TABLE sessions ADD COLUMN exercise_order_in_stage INTEGER DEFAULT 1";

    if (!executeSQL(add_stage)) {
        std::cerr << "[SessionDatabase] ⚠️  Не удалось добавить exercise_stage (возможно уже существует)" << std::endl;
    }

    if (!executeSQL(add_order)) {
        std::cerr << "[SessionDatabase] ⚠️  Не удалось добавить exercise_order_in_stage (возможно уже существует)" << std::endl;
    }

    std::cout << "[SessionDatabase] ✓ Миграция завершена" << std::endl;
    return true;
}

std::vector<SessionRecord> SessionDatabase::getSessionsByStage(
    int stage, const std::string& user_id) const {

    std::vector<SessionRecord> sessions;
    if (!m_db) return sessions;

    const char* query = R"(
        SELECT id, user_id, exercise_name, timestamp, duration_seconds, success_rate,
               exercise_stage, exercise_order_in_stage,
               avg_alpha, avg_beta, avg_theta,
               avg_concentration, avg_relaxation, avg_fatigue,
               avg_focus, avg_stress, avg_heart_rate, csv_file_path
        FROM sessions
        WHERE user_id = ? AND exercise_stage = ?
        ORDER BY timestamp DESC;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        return sessions;
    }

    sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, stage);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        SessionRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.user_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.exercise_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        record.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        record.duration_seconds = sqlite3_column_int(stmt, 4);
        record.success_rate = sqlite3_column_double(stmt, 5);
        record.exercise_stage = sqlite3_column_int(stmt, 6);
        record.exercise_order_in_stage = sqlite3_column_int(stmt, 7);
        record.avg_alpha = sqlite3_column_double(stmt, 8);
        record.avg_beta = sqlite3_column_double(stmt, 9);
        record.avg_theta = sqlite3_column_double(stmt, 10);
        record.avg_concentration = sqlite3_column_double(stmt, 11);
        record.avg_relaxation = sqlite3_column_double(stmt, 12);
        record.avg_fatigue = sqlite3_column_double(stmt, 13);
        record.avg_focus = sqlite3_column_double(stmt, 14);
        record.avg_stress = sqlite3_column_double(stmt, 15);
        record.avg_heart_rate = sqlite3_column_double(stmt, 16);
        if (sqlite3_column_text(stmt, 17)) {
            record.csv_file_path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
        }
        sessions.push_back(record);
    }

    sqlite3_finalize(stmt);
    return sessions;
}

StageStats SessionDatabase::getStageStats(int stage, const std::string& user_id) const {
    StageStats stats;
    stats.stage = stage;

    if (!m_db) return stats;

    // Количество упражнений на ступени (из кода)
    const int exercises_per_stage[] = {10, 25, 15, 10};  // Подг, 1-я, 2-я, 3-я
    stats.total_exercises_in_stage = (stage < 4) ? exercises_per_stage[stage] : 0;

    // Подсчет освоенных упражнений (success >= 70%)
    const char* completed_query = R"(
        SELECT COUNT(DISTINCT exercise_order_in_stage)
        FROM sessions
        WHERE user_id = ? AND exercise_stage = ? AND success_rate >= 70.0;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, completed_query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, stage);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.completed_exercises = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    // Общее количество сессий и средний success rate
    const char* stats_query = R"(
        SELECT COUNT(*), AVG(success_rate)
        FROM sessions
        WHERE user_id = ? AND exercise_stage = ?;
    )";

    if (sqlite3_prepare_v2(m_db, stats_query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user_id.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, stage);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.total_sessions = sqlite3_column_int(stmt, 0);
            stats.avg_success_rate = sqlite3_column_double(stmt, 1);
        }
        sqlite3_finalize(stmt);
    }

    // Процент освоения
    if (stats.total_exercises_in_stage > 0) {
        stats.completion_percentage =
            (100.0 * stats.completed_exercises) / stats.total_exercises_in_stage;
    }

    return stats;
}

std::vector<StageStats> SessionDatabase::getAllStagesProgress(const std::string& user_id) const {
    std::vector<StageStats> all_progress;

    // Получаем статистику по всем 4 ступеням
    for (int stage = 0; stage <= 3; ++stage) {
        StageStats stats = getStageStats(stage, user_id);
        all_progress.push_back(stats);
    }

    return all_progress;
}

} // namespace Bronnikov
