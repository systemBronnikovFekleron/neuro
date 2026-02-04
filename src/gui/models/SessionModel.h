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

    // Q_INVOKABLE methods
    Q_INVOKABLE void loadStatistics(const QString& userId = "default");
    Q_INVOKABLE void loadStageProgress(const QString& userId = "default");
    Q_INVOKABLE void loadRecentSessions(int limit = 10, const QString& userId = "default");
    Q_INVOKABLE void loadSessionsByExercise(const QString& exerciseName, const QString& userId = "default");

    Q_INVOKABLE bool saveSession(const QVariantMap& sessionData);
    Q_INVOKABLE QVariantMap getSessionById(int sessionId) const;

    Q_INVOKABLE void setDatabase(void* database);  // Accepts SessionDatabase*

signals:
    void statisticsChanged();
    void stageProgressChanged();
    void recentSessionsChanged();
    void sessionSaved(int sessionId);

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

    // Reference to C++ SessionDatabase
    Bronnikov::SessionDatabase* m_database = nullptr;

    void updateStatistics();
    QVariantMap recordToVariantMap(const Bronnikov::SessionRecord& record) const;
};
