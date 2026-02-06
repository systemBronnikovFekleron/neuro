#include "models/SessionModel.h"
#include "SessionDatabase.h"
#include <QDateTime>
#include <QDebug>
#include <map>

SessionModel::SessionModel(QObject* parent)
    : QObject(parent)
{
}

SessionModel::~SessionModel() = default;

QVariantList SessionModel::stageProgress() const
{
    QVariantList result;
    for (const auto& progress : m_stageProgressData) {
        QVariantMap item;
        item["stage"] = progress.stage;
        item["totalExercises"] = progress.totalExercises;
        item["completedExercises"] = progress.completedExercises;
        item["totalSessions"] = progress.totalSessions;
        item["avgSuccessRate"] = progress.avgSuccessRate;
        item["completionPercentage"] = progress.completionPercentage;
        result.append(item);
    }
    return result;
}

QVariantList SessionModel::recentSessions() const
{
    QVariantList result;
    for (const auto& session : m_recentSessionsData) {
        result.append(session);
    }
    return result;
}

void SessionModel::loadStatistics(const QString& userId)
{
    if (!m_database)
        return;

    auto stats = m_database->getUserStats(userId.toStdString());

    m_totalSessions = stats.total_sessions;
    m_totalHours = stats.total_hours;
    m_avgSuccessRate = stats.avg_success_rate;
    m_firstSessionDate = QString::fromStdString(stats.first_session_date);
    m_lastSessionDate = QString::fromStdString(stats.last_session_date);

    emit statisticsChanged();
}

void SessionModel::loadStageProgress(const QString& userId)
{
    if (!m_database)
        return;

    m_stageProgressData.clear();

    auto all_progress = m_database->getAllStagesProgress(userId.toStdString());

    for (const auto& progress : all_progress) {
        StageProgressData data;
        data.stage = progress.stage;
        data.totalExercises = progress.total_exercises_in_stage;
        data.completedExercises = progress.completed_exercises;
        data.totalSessions = progress.total_sessions;
        data.avgSuccessRate = progress.avg_success_rate;
        data.completionPercentage = progress.completion_percentage;

        m_stageProgressData.push_back(data);
    }

    emit stageProgressChanged();
}

void SessionModel::loadRecentSessions(int limit, const QString& userId)
{
    if (!m_database)
        return;

    m_recentSessionsData.clear();

    auto sessions = m_database->getSessions(userId.toStdString(), limit);

    for (const auto& record : sessions) {
        m_recentSessionsData.push_back(recordToVariantMap(record));
    }

    emit recentSessionsChanged();
}

void SessionModel::loadSessionsByExercise(const QString& exerciseName, const QString& userId)
{
    if (!m_database)
        return;

    m_recentSessionsData.clear();

    auto sessions = m_database->getSessionsByExercise(exerciseName.toStdString(), userId.toStdString());

    for (const auto& record : sessions) {
        m_recentSessionsData.push_back(recordToVariantMap(record));
    }

    emit recentSessionsChanged();
}

bool SessionModel::saveSession(const QVariantMap& sessionData)
{
    if (!m_database)
        return false;

    Bronnikov::SessionRecord record;
    record.user_id = sessionData["userId"].toString().toStdString();
    record.exercise_name = sessionData["exerciseName"].toString().toStdString();
    record.timestamp = sessionData["timestamp"].toString().toStdString();
    record.duration_seconds = sessionData["durationSeconds"].toInt();
    record.success_rate = sessionData["successRate"].toDouble();
    record.exercise_stage = sessionData["exerciseStage"].toInt();
    record.exercise_order_in_stage = sessionData["exerciseOrder"].toInt();

    bool success = m_database->saveSession(record);

    if (success) {
        emit sessionSaved(record.id);
    }

    return success;
}

QVariantMap SessionModel::getSessionById(int sessionId) const
{
    // TODO: Implement when needed
    Q_UNUSED(sessionId);
    return QVariantMap();
}

void SessionModel::setDatabase(void* database)
{
    m_database = static_cast<Bronnikov::SessionDatabase*>(database);
}

void SessionModel::updateStatistics()
{
    loadStatistics("default");
}

QVariantMap SessionModel::recordToVariantMap(const Bronnikov::SessionRecord& record) const
{
    QVariantMap result;
    result["id"] = record.id;
    result["userId"] = QString::fromStdString(record.user_id);
    result["exerciseName"] = QString::fromStdString(record.exercise_name);
    result["timestamp"] = QString::fromStdString(record.timestamp);
    result["durationSeconds"] = record.duration_seconds;
    result["successRate"] = record.success_rate;
    result["exerciseStage"] = record.exercise_stage;
    result["exerciseOrder"] = record.exercise_order_in_stage;
    result["avgAlpha"] = record.avg_alpha;
    result["avgBeta"] = record.avg_beta;
    result["avgTheta"] = record.avg_theta;
    result["avgConcentration"] = record.avg_concentration;
    result["avgRelaxation"] = record.avg_relaxation;
    result["avgFatigue"] = record.avg_fatigue;
    result["avgHeartRate"] = record.avg_heart_rate;
    return result;
}

void SessionModel::loadUserProfile(const QString& userId)
{
    if (!m_database)
        return;

    m_currentUserId = userId;  // Сохраняем текущий userId
    auto profile = m_database->loadUserProfile(userId.toStdString());

    if (profile.user_id.empty()) {
        // Профиль не найден - устанавливаем значения по умолчанию
        m_userName = userId;
        m_practiceLevel = 0;  // Beginner
        m_iaf = 0.0;
        m_iapf = 0.0;
        m_lastCalibrationDate = "";
        m_baselineAlpha = 0.0;
        m_baselineBeta = 0.0;
        m_baselineTheta = 0.0;
        m_baselineConcentration = 0.0;
        m_baselineRelaxation = 0.0;
        m_baselineHeartRate = 0.0;
    } else {
        // Загружаем данные из БД
        m_userName = QString::fromStdString(profile.name);
        m_practiceLevel = static_cast<int>(profile.practice_level);
        m_iaf = profile.iaf;
        m_iapf = profile.iapf;
        m_lastCalibrationDate = QString::fromStdString(profile.last_calibration_date);
        m_baselineAlpha = profile.baseline_alpha;
        m_baselineBeta = profile.baseline_beta;
        m_baselineTheta = profile.baseline_theta;
        m_baselineConcentration = profile.baseline_concentration;
        m_baselineRelaxation = profile.baseline_relaxation;
        m_baselineHeartRate = profile.baseline_heart_rate;
    }

    emit userProfileChanged();
}

void SessionModel::setPracticeLevel(int level, const QString& userId)
{
    if (!m_database)
        return;

    // Загружаем текущий профиль
    auto profile = m_database->loadUserProfile(userId.toStdString());

    if (profile.user_id.empty()) {
        // Профиль не найден - создаем новый
        profile.user_id = userId.toStdString();
        profile.name = "Пользователь";
    }

    // Обновляем уровень практики
    profile.practice_level = static_cast<Bronnikov::PracticeLevel>(level);

    // Сохраняем в БД
    if (m_database->saveUserProfile(profile)) {
        m_practiceLevel = level;
        emit userProfileChanged();
    }
}

void SessionModel::updateCalibration(double iaf, double iapf)
{
    m_iaf = iaf;
    m_iapf = iapf;

    // Обновляем дату калибровки
    m_lastCalibrationDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    emit userProfileChanged();

    // Сохраняем в БД
    if (m_database) {
        auto profile = m_database->loadUserProfile(m_currentUserId.toStdString());
        if (profile.user_id.empty()) {
            profile.user_id = m_currentUserId.toStdString();
            profile.name = m_userName.isEmpty() ? "Пользователь" : m_userName.toStdString();
        }
        profile.iaf = iaf;
        profile.iapf = iapf;
        profile.last_calibration_date = m_lastCalibrationDate.toStdString();
        m_database->saveUserProfile(profile);
        qDebug() << "[SessionModel] Калибровка сохранена: IAF=" << iaf << "IAPF=" << iapf;
    }
}

void SessionModel::updateBaseline(const QVariantMap& baselineMetrics)
{
    m_baselineAlpha = baselineMetrics["alpha"].toDouble();
    m_baselineBeta = baselineMetrics["beta"].toDouble();
    m_baselineTheta = baselineMetrics["theta"].toDouble();
    m_baselineConcentration = baselineMetrics["concentration"].toDouble();
    m_baselineRelaxation = baselineMetrics["relaxation"].toDouble();
    m_baselineHeartRate = baselineMetrics["heartRate"].toDouble();

    emit userProfileChanged();

    // Сохраняем в БД
    if (m_database) {
        auto profile = m_database->loadUserProfile(m_currentUserId.toStdString());
        if (profile.user_id.empty()) {
            profile.user_id = m_currentUserId.toStdString();
            profile.name = m_userName.isEmpty() ? "Пользователь" : m_userName.toStdString();
        }
        profile.baseline_alpha = m_baselineAlpha;
        profile.baseline_beta = m_baselineBeta;
        profile.baseline_theta = m_baselineTheta;
        profile.baseline_concentration = m_baselineConcentration;
        profile.baseline_relaxation = m_baselineRelaxation;
        profile.baseline_heart_rate = m_baselineHeartRate;
        m_database->saveUserProfile(profile);
        qDebug() << "[SessionModel] Baseline сохранён: Alpha=" << m_baselineAlpha
                 << "Beta=" << m_baselineBeta << "Theta=" << m_baselineTheta;
    }
}

QVariantList SessionModel::getExercisesStatistics(const QString& userId)
{
    QVariantList result;
    if (!m_database)
        return result;

    // Получаем все сессии пользователя
    auto allSessions = m_database->getSessions(userId.toStdString(), 0);

    // Группируем по упражнениям
    std::map<std::string, std::vector<Bronnikov::SessionRecord>> byExercise;
    for (const auto& session : allSessions) {
        byExercise[session.exercise_name].push_back(session);
    }

    // Формируем статистику по каждому упражнению
    for (const auto& [name, sessions] : byExercise) {
        QVariantMap item;
        item["exerciseName"] = QString::fromStdString(name);
        item["sessionCount"] = static_cast<int>(sessions.size());

        // Вычисляем средний success rate и лучший результат
        double totalSuccess = 0.0;
        double bestSuccess = 0.0;
        int totalDuration = 0;
        int stage = 0;
        int order = 0;

        for (const auto& s : sessions) {
            totalSuccess += s.success_rate;
            if (s.success_rate > bestSuccess) {
                bestSuccess = s.success_rate;
            }
            totalDuration += s.duration_seconds;
            stage = s.exercise_stage;
            order = s.exercise_order_in_stage;
        }

        item["avgSuccessRate"] = sessions.empty() ? 0.0 : totalSuccess / sessions.size();
        item["bestSuccessRate"] = bestSuccess;
        item["totalMinutes"] = totalDuration / 60;
        item["stage"] = stage;
        item["order"] = order;
        item["lastSessionDate"] = sessions.empty() ? "" :
            QString::fromStdString(sessions.front().timestamp);

        result.append(item);
    }

    return result;
}

QVariantList SessionModel::getSessionsByDay(int daysBack, const QString& userId)
{
    QVariantList result;
    if (!m_database)
        return result;

    // Получаем все сессии
    auto allSessions = m_database->getSessions(userId.toStdString(), 0);

    // Группируем по дням
    std::map<QString, int> sessionsByDay;
    QDateTime now = QDateTime::currentDateTime();
    QDateTime cutoff = now.addDays(-daysBack);

    for (const auto& session : allSessions) {
        QDateTime sessionTime = QDateTime::fromString(
            QString::fromStdString(session.timestamp), "yyyy-MM-dd HH:mm:ss");
        if (sessionTime >= cutoff) {
            QString dateStr = sessionTime.date().toString("yyyy-MM-dd");
            sessionsByDay[dateStr]++;
        }
    }

    // Формируем результат
    for (const auto& [date, count] : sessionsByDay) {
        QVariantMap item;
        item["date"] = date;
        item["sessionCount"] = count;
        result.append(item);
    }

    return result;
}

QStringList SessionModel::getAllUsers()
{
    QStringList result;
    if (!m_database)
        return result;

    auto users = m_database->getAllUserIds();
    for (const auto& userId : users) {
        result.append(QString::fromStdString(userId));
    }

    return result;
}

bool SessionModel::createUser(const QString& userId, const QString& userName)
{
    if (!m_database)
        return false;

    if (userId.isEmpty())
        return false;

    // Создаём новый профиль пользователя
    Bronnikov::UserProfileData profile;
    profile.user_id = userId.toStdString();
    profile.name = userName.isEmpty() ? userId.toStdString() : userName.toStdString();
    profile.practice_level = Bronnikov::PracticeLevel::Beginner;

    bool success = m_database->saveUserProfile(profile);

    if (success) {
        qDebug() << "[SessionModel] Пользователь создан:" << userId;
    }

    return success;
}

void SessionModel::switchUser(const QString& userId)
{
    if (userId.isEmpty())
        return;

    qDebug() << "[SessionModel] Переключение на пользователя:" << userId;

    // Загружаем профиль нового пользователя
    loadUserProfile(userId);

    // Загружаем статистику
    loadStatistics(userId);
    loadStageProgress(userId);
    loadRecentSessions(5, userId);
}
