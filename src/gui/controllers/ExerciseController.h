#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QTimer>
#include <memory>

// Forward declarations
namespace Bronnikov {
    class Exercise;
    class SessionManager;
    class MetricsCollector;
    class SessionDatabase;
    struct MetricsSnapshot;
}

/**
 * @brief Qt Controller для управления выполнением упражнений
 *
 * Координирует выполнение упражнения:
 * - Запуск/остановка/пауза упражнения
 * - Отслеживание фаз упражнения
 * - Обновление инструкций
 * - Расчет Success Rate в реальном времени
 * - Сохранение результатов в БД
 */
class ExerciseController : public QObject
{
    Q_OBJECT

    // Exercise state properties
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(QString exerciseName READ exerciseName NOTIFY exerciseNameChanged)
    Q_PROPERTY(QString currentPhase READ currentPhase NOTIFY currentPhaseChanged)
    Q_PROPERTY(QString currentInstruction READ currentInstruction NOTIFY currentInstructionChanged)

    // Timing properties
    Q_PROPERTY(int elapsedSeconds READ elapsedSeconds NOTIFY elapsedSecondsChanged)
    Q_PROPERTY(int totalSeconds READ totalSeconds NOTIFY totalSecondsChanged)
    Q_PROPERTY(int progressPercentage READ progressPercentage NOTIFY progressPercentageChanged)

    // Performance properties
    Q_PROPERTY(double currentSuccessRate READ currentSuccessRate NOTIFY currentSuccessRateChanged)
    Q_PROPERTY(double targetSuccessRate READ targetSuccessRate NOTIFY targetSuccessRateChanged)

    // Target metrics (what user should achieve)
    Q_PROPERTY(QVariantMap targetMetrics READ targetMetrics NOTIFY targetMetricsChanged)

public:
    explicit ExerciseController(QObject* parent = nullptr);
    ~ExerciseController() override;

    // Property getters
    bool isActive() const { return m_isActive; }
    bool isPaused() const { return m_isPaused; }
    QString exerciseName() const { return m_exerciseName; }
    QString currentPhase() const { return m_currentPhase; }
    QString currentInstruction() const { return m_currentInstruction; }
    int elapsedSeconds() const { return m_elapsedSeconds; }
    int totalSeconds() const { return m_totalSeconds; }
    int progressPercentage() const;
    double currentSuccessRate() const { return m_currentSuccessRate; }
    double targetSuccessRate() const { return m_targetSuccessRate; }
    QVariantMap targetMetrics() const;

    // Setters для C++ интеграции
    void setSessionManager(Bronnikov::SessionManager* manager);
    void setMetricsCollector(Bronnikov::MetricsCollector* collector);
    void setDatabase(Bronnikov::SessionDatabase* database);

    // Q_INVOKABLE methods
    Q_INVOKABLE void startExercise(const QString& exerciseName);
    Q_INVOKABLE void stopExercise();
    Q_INVOKABLE void pauseExercise();
    Q_INVOKABLE void resumeExercise();

    Q_INVOKABLE QVariantMap getExerciseResults() const;
    Q_INVOKABLE QVariantMap getSessionSummary() const;

signals:
    // Property change signals
    void isActiveChanged();
    void isPausedChanged();
    void exerciseNameChanged();
    void currentPhaseChanged();
    void currentInstructionChanged();
    void elapsedSecondsChanged();
    void totalSecondsChanged();
    void progressPercentageChanged();
    void currentSuccessRateChanged();
    void targetSuccessRateChanged();
    void targetMetricsChanged();

    // Event signals
    void exerciseStarted(const QString& name);
    void exerciseCompleted(double successRate);
    void exerciseFailed(const QString& error);
    void phaseChanged(const QString& phaseName);
    void instructionChanged(const QString& instruction);

private slots:
    // Timer slot для обновления прогресса
    void updateProgress();

    // Slots для обработки событий упражнения
    void onExercisePhaseChanged();
    void onMetricsUpdated(const Bronnikov::MetricsSnapshot& metrics);

private:
    // Properties state
    bool m_isActive = false;
    bool m_isPaused = false;
    QString m_exerciseName;
    QString m_currentPhase;
    QString m_currentInstruction;
    int m_elapsedSeconds = 0;
    int m_totalSeconds = 0;
    double m_currentSuccessRate = 0.0;
    double m_targetSuccessRate = 70.0;  // Default target

    // C++ business logic references
    Bronnikov::SessionManager* m_sessionManager = nullptr;
    Bronnikov::MetricsCollector* m_metricsCollector = nullptr;
    Bronnikov::SessionDatabase* m_database = nullptr;

    // Current exercise instance
    std::shared_ptr<Bronnikov::Exercise> m_currentExercise;

    // Update timer (every second)
    QTimer* m_progressTimer = nullptr;

    // Internal helpers
    void saveSessionToDatabase();
    void resetState();
    void updateSuccessRate();
    double calculateSuccessRate(const Bronnikov::MetricsSnapshot& metrics);
};
