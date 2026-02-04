#include "models/SessionModel.h"
#include "SessionDatabase.h"

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
