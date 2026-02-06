#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <memory>
#include <vector>

// Forward declarations
namespace Bronnikov {
    class SessionDatabase;
    struct SessionRecord;
}

/**
 * @brief Qt Model для работы с историей сессий и статистикой
 *
 * Предоставляет доступ к SessionDatabase через Qt-friendly интерфейс.
 * Используется в StatisticsScreen и ResultsScreen для отображения истории и аналитики.
 */
class SessionModel : public QObject
{
    Q_OBJECT

    // Overall statistics properties
    Q_PROPERTY(int totalSessions READ totalSessions NOTIFY statisticsChanged)
    Q_PROPERTY(double totalHours READ totalHours NOTIFY statisticsChanged)
    Q_PROPERTY(double avgSuccessRate READ avgSuccessRate NOTIFY statisticsChanged)
    Q_PROPERTY(QString firstSessionDate READ firstSessionDate NOTIFY statisticsChanged)
    Q_PROPERTY(QString lastSessionDate READ lastSessionDate NOTIFY statisticsChanged)

    // Stage progress properties
    Q_PROPERTY(QVariantList stageProgress READ stageProgress NOTIFY stageProgressChanged)

    // Recent sessions
    Q_PROPERTY(QVariantList recentSessions READ recentSessions NOTIFY recentSessionsChanged)

    // User profile properties
    Q_PROPERTY(QString userName READ userName NOTIFY userProfileChanged)
    Q_PROPERTY(int practiceLevel READ practiceLevel NOTIFY userProfileChanged)
    Q_PROPERTY(double iaf READ iaf NOTIFY userProfileChanged)
    Q_PROPERTY(double iapf READ iapf NOTIFY userProfileChanged)
    Q_PROPERTY(QString lastCalibrationDate READ lastCalibrationDate NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineAlpha READ baselineAlpha NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineBeta READ baselineBeta NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineTheta READ baselineTheta NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineConcentration READ baselineConcentration NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineRelaxation READ baselineRelaxation NOTIFY userProfileChanged)
    Q_PROPERTY(double baselineHeartRate READ baselineHeartRate NOTIFY userProfileChanged)

public:
    explicit SessionModel(QObject* parent = nullptr);
    ~SessionModel() override;

    // Property getters
    int totalSessions() const { return m_totalSessions; }
    double totalHours() const { return m_totalHours; }
    double avgSuccessRate() const { return m_avgSuccessRate; }
    QString firstSessionDate() const { return m_firstSessionDate; }
    QString lastSessionDate() const { return m_lastSessionDate; }
    QVariantList stageProgress() const;
    QVariantList recentSessions() const;

    // User profile getters
    QString userName() const { return m_userName; }
    int practiceLevel() const { return m_practiceLevel; }
    double iaf() const { return m_iaf; }
    double iapf() const { return m_iapf; }
    QString lastCalibrationDate() const { return m_lastCalibrationDate; }
    double baselineAlpha() const { return m_baselineAlpha; }
    double baselineBeta() const { return m_baselineBeta; }
    double baselineTheta() const { return m_baselineTheta; }
    double baselineConcentration() const { return m_baselineConcentration; }
    double baselineRelaxation() const { return m_baselineRelaxation; }
    double baselineHeartRate() const { return m_baselineHeartRate; }

    // Q_INVOKABLE methods
    Q_INVOKABLE void loadStatistics(const QString& userId = "default");
    Q_INVOKABLE void loadStageProgress(const QString& userId = "default");
    Q_INVOKABLE void loadRecentSessions(int limit = 10, const QString& userId = "default");
    Q_INVOKABLE void loadSessionsByExercise(const QString& exerciseName, const QString& userId = "default");

    Q_INVOKABLE bool saveSession(const QVariantMap& sessionData);
    Q_INVOKABLE QVariantMap getSessionById(int sessionId) const;

    // Exercise statistics for StatisticsScreen
    Q_INVOKABLE QVariantList getExercisesStatistics(const QString& userId = "default");
    Q_INVOKABLE QVariantList getSessionsByDay(int daysBack = 30, const QString& userId = "default");

    Q_INVOKABLE void setDatabase(void* database);  // Accepts SessionDatabase*

    // User profile methods
    Q_INVOKABLE void loadUserProfile(const QString& userId = "default");
    Q_INVOKABLE void setPracticeLevel(int level, const QString& userId = "default");
    Q_INVOKABLE void updateCalibration(double iaf, double iapf);
    Q_INVOKABLE void updateBaseline(const QVariantMap& baselineMetrics);

    // User management methods
    Q_INVOKABLE QStringList getAllUsers();
    Q_INVOKABLE bool createUser(const QString& userId, const QString& userName = "");
    Q_INVOKABLE void switchUser(const QString& userId);
    Q_INVOKABLE QString currentUserId() const { return m_currentUserId; }

signals:
    void statisticsChanged();
    void stageProgressChanged();
    void recentSessionsChanged();
    void sessionSaved(int sessionId);
    void userProfileChanged();

private:
    // Statistics data
    int m_totalSessions = 0;
    double m_totalHours = 0.0;
    double m_avgSuccessRate = 0.0;
    QString m_firstSessionDate;
    QString m_lastSessionDate;

    // Stage progress data
    struct StageProgressData {
        int stage = 0;
        int totalExercises = 0;
        int completedExercises = 0;
        int totalSessions = 0;
        double avgSuccessRate = 0.0;
        double completionPercentage = 0.0;
    };
    std::vector<StageProgressData> m_stageProgressData;

    // Recent sessions
    std::vector<QVariantMap> m_recentSessionsData;

    // User profile data
    QString m_currentUserId = "default";  // Текущий пользователь
    QString m_userName;
    int m_practiceLevel = 0;  // 0=Beginner, 1=Intermediate, 2=Expert
    QString m_lastCalibrationDate;
    double m_iaf = 0.0;
    double m_iapf = 0.0;
    double m_baselineAlpha = 0.0;
    double m_baselineBeta = 0.0;
    double m_baselineTheta = 0.0;
    double m_baselineConcentration = 0.0;
    double m_baselineRelaxation = 0.0;
    double m_baselineHeartRate = 0.0;

    // Reference to C++ SessionDatabase
    Bronnikov::SessionDatabase* m_database = nullptr;

    void updateStatistics();
    QVariantMap recordToVariantMap(const Bronnikov::SessionRecord& record) const;
};
