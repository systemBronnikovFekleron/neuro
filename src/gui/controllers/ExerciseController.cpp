#include "controllers/ExerciseController.h"
#include "SessionManager.h"
#include "MetricsCollector.h"
#include "SessionDatabase.h"
#include "Exercise.h"

ExerciseController::ExerciseController(QObject* parent)
    : QObject(parent)
    , m_progressTimer(new QTimer(this))
{
    connect(m_progressTimer, &QTimer::timeout, this, &ExerciseController::updateProgress);
}

ExerciseController::~ExerciseController() = default;

int ExerciseController::progressPercentage() const
{
    if (m_totalSeconds == 0)
        return 0;
    return static_cast<int>((m_elapsedSeconds * 100.0) / m_totalSeconds);
}

QVariantMap ExerciseController::targetMetrics() const
{
    QVariantMap result;
    // TODO: Get from current exercise
    result["targetAlpha"] = 70.0;
    result["targetBeta"] = 30.0;
    result["targetTheta"] = 20.0;
    return result;
}

void ExerciseController::setSessionManager(Bronnikov::SessionManager* manager)
{
    m_sessionManager = manager;
}

void ExerciseController::setMetricsCollector(Bronnikov::MetricsCollector* collector)
{
    m_metricsCollector = collector;
}

void ExerciseController::setDatabase(Bronnikov::SessionDatabase* database)
{
    m_database = database;
}

void ExerciseController::startExercise(const QString& exerciseName)
{
    if (m_isActive) {
        emit exerciseFailed("Another exercise is already active");
        return;
    }

    // TODO: Load exercise from ExerciseLibrary
    // For now, simulate
    m_exerciseName = exerciseName;
    m_totalSeconds = 600;  // 10 minutes
    m_elapsedSeconds = 0;
    m_currentPhase = "Preparation";
    m_currentInstruction = "Prepare for the exercise";
    m_currentSuccessRate = 0.0;
    m_isActive = true;

    m_progressTimer->start(1000);  // Update every second

    emit isActiveChanged();
    emit exerciseNameChanged();
    emit currentPhaseChanged();
    emit currentInstructionChanged();
    emit totalSecondsChanged();
    emit exerciseStarted(exerciseName);
}

void ExerciseController::stopExercise()
{
    if (!m_isActive)
        return;

    m_progressTimer->stop();

    // Save session
    saveSessionToDatabase();

    double finalSuccessRate = m_currentSuccessRate;

    resetState();

    emit exerciseCompleted(finalSuccessRate);
}

void ExerciseController::pauseExercise()
{
    if (!m_isActive || m_isPaused)
        return;

    m_isPaused = true;
    m_progressTimer->stop();
    emit isPausedChanged();
}

void ExerciseController::resumeExercise()
{
    if (!m_isActive || !m_isPaused)
        return;

    m_isPaused = false;
    m_progressTimer->start(1000);
    emit isPausedChanged();
}

QVariantMap ExerciseController::getExerciseResults() const
{
    QVariantMap results;
    results["exerciseName"] = m_exerciseName;
    results["duration"] = m_elapsedSeconds;
    results["successRate"] = m_currentSuccessRate;
    return results;
}

QVariantMap ExerciseController::getSessionSummary() const
{
    QVariantMap summary;
    summary["exerciseName"] = m_exerciseName;
    summary["totalSeconds"] = m_totalSeconds;
    summary["elapsedSeconds"] = m_elapsedSeconds;
    summary["successRate"] = m_currentSuccessRate;
    summary["phase"] = m_currentPhase;
    return summary;
}

void ExerciseController::updateProgress()
{
    if (!m_isActive || m_isPaused)
        return;

    m_elapsedSeconds++;
    emit elapsedSecondsChanged();
    emit progressPercentageChanged();

    // Update success rate
    if (m_metricsCollector) {
        auto metrics = m_metricsCollector->getCurrentMetrics();
        m_currentSuccessRate = calculateSuccessRate(metrics);
        emit currentSuccessRateChanged();
    }

    // Check if exercise completed
    if (m_elapsedSeconds >= m_totalSeconds) {
        stopExercise();
    }
}

void ExerciseController::onExercisePhaseChanged()
{
    // TODO: Handle phase changes from Exercise
}

void ExerciseController::onMetricsUpdated(const Bronnikov::MetricsSnapshot& metrics)
{
    m_currentSuccessRate = calculateSuccessRate(metrics);
    emit currentSuccessRateChanged();
}

void ExerciseController::saveSessionToDatabase()
{
    if (!m_database)
        return;

    Bronnikov::SessionRecord record;
    record.user_id = "default";
    record.exercise_name = m_exerciseName.toStdString();
    record.duration_seconds = m_elapsedSeconds;
    record.success_rate = m_currentSuccessRate;

    // TODO: Get from exercise
    record.exercise_stage = 0;  // Preparatory
    record.exercise_order_in_stage = 1;

    if (m_metricsCollector) {
        auto metrics = m_metricsCollector->getCurrentMetrics();
        record.avg_alpha = metrics.alpha_power;
        record.avg_beta = metrics.beta_power;
        record.avg_theta = metrics.theta_power;
        record.avg_concentration = metrics.concentration;
        record.avg_relaxation = metrics.relaxation;
        record.avg_fatigue = metrics.fatigue;
        record.avg_heart_rate = metrics.heart_rate;
    }

    m_database->saveSession(record);
}

void ExerciseController::resetState()
{
    m_isActive = false;
    m_isPaused = false;
    m_exerciseName.clear();
    m_currentPhase.clear();
    m_currentInstruction.clear();
    m_elapsedSeconds = 0;
    m_totalSeconds = 0;
    m_currentSuccessRate = 0.0;
    m_currentExercise.reset();

    emit isActiveChanged();
    emit isPausedChanged();
    emit exerciseNameChanged();
    emit currentPhaseChanged();
    emit currentInstructionChanged();
    emit elapsedSecondsChanged();
    emit totalSecondsChanged();
    emit currentSuccessRateChanged();
}

void ExerciseController::updateSuccessRate()
{
    if (!m_metricsCollector)
        return;

    auto metrics = m_metricsCollector->getCurrentMetrics();
    m_currentSuccessRate = calculateSuccessRate(metrics);
    emit currentSuccessRateChanged();
}

double ExerciseController::calculateSuccessRate(const Bronnikov::MetricsSnapshot& metrics)
{
    // Simple calculation - TODO: Improve based on exercise targets
    double rate = 0.0;
    rate += metrics.concentration * 0.3;
    rate += metrics.relaxation * 0.2;
    rate += (100.0 - metrics.fatigue) * 0.2;
    rate += metrics.alpha_power * 0.3;

    return std::min(100.0, std::max(0.0, rate));
}
