#include "controllers/ExerciseController.h"
#include "SessionManager.h"
#include "MetricsCollector.h"
#include "SessionDatabase.h"
#include "Exercise.h"
#include "ExerciseLibrary.h"
#include <QDebug>

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

    if (m_currentExercise) {
        auto targets = m_currentExercise->getTargetMetrics();
        // Use midpoint of ranges as target values
        result["targetAlpha"] = (targets.alpha_min + targets.alpha_max) / 2.0;
        result["targetBeta"] = (targets.beta_min + targets.beta_max) / 2.0;
        result["targetTheta"] = (targets.theta_min + targets.theta_max) / 2.0;
        result["targetConcentration"] = (targets.concentration_min + targets.concentration_max) / 2.0;
        result["targetRelaxation"] = (targets.relaxation_min + targets.relaxation_max) / 2.0;
        result["concentrationChange"] = targets.concentration_change;
        result["relaxationChange"] = targets.relaxation_change;
        result["alphaChange"] = targets.alpha_change;
    } else {
        // Default values
        result["targetAlpha"] = 70.0;
        result["targetBeta"] = 30.0;
        result["targetTheta"] = 20.0;
        result["targetConcentration"] = 60.0;
        result["targetRelaxation"] = 50.0;
    }

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

    // Load exercise from ExerciseLibrary
    Bronnikov::ExerciseLibrary library;
    m_currentExercise = library.getExerciseByName(exerciseName.toStdString());

    if (!m_currentExercise) {
        qWarning() << "[ExerciseController] Exercise not found:" << exerciseName;
        emit exerciseFailed("Exercise not found: " + exerciseName);
        return;
    }

    qDebug() << "[ExerciseController] Starting exercise:" << exerciseName
             << "Stage:" << static_cast<int>(m_currentExercise->getStage())
             << "Order:" << m_currentExercise->getOrderInStage();

    m_exerciseName = exerciseName;

    // Use configured duration or exercise default
    if (m_duration > 0) {
        m_currentExercise->setDuration(m_duration);
    }
    m_totalSeconds = m_currentExercise->getDuration() * 60;

    m_elapsedSeconds = 0;
    m_currentPhase = "Baseline";
    auto instruction = m_currentExercise->getCurrentInstruction();
    m_currentInstruction = instruction ? QString::fromStdString(instruction->text) : "Подготовка к упражнению";
    m_currentSuccessRate = 0.0;
    m_isActive = true;

    // Start baseline phase for the exercise
    m_currentExercise->startBaseline();

    m_progressTimer->start(1000);  // Update every second

    emit isActiveChanged();
    emit exerciseNameChanged();
    emit currentPhaseChanged();
    emit currentInstructionChanged();
    emit totalSecondsChanged();
    emit targetMetricsChanged();
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

    // Get stage info from exercise
    if (m_currentExercise) {
        record.exercise_stage = static_cast<int>(m_currentExercise->getStage());
        record.exercise_order_in_stage = m_currentExercise->getOrderInStage();
    } else {
        record.exercise_stage = 0;
        record.exercise_order_in_stage = 1;
    }

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
    qDebug() << "[ExerciseController] Session saved - Stage:" << record.exercise_stage
             << "Order:" << record.exercise_order_in_stage
             << "Success:" << record.success_rate << "%";
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
    // If exercise has target metrics, use weighted comparison
    if (m_currentExercise) {
        auto targets = m_currentExercise->getTargetMetrics();

        double score = 0.0;
        double totalWeight = 0.0;

        // Concentration comparison (using weights from targets)
        double concTarget = (targets.concentration_min + targets.concentration_max) / 2.0;
        if (concTarget > 0) {
            double concScore = std::min(100.0, (metrics.concentration / concTarget) * 100.0);
            score += concScore * targets.concentration_weight;
            totalWeight += targets.concentration_weight;
        }

        // Relaxation comparison
        double relaxTarget = (targets.relaxation_min + targets.relaxation_max) / 2.0;
        if (relaxTarget > 0) {
            double relaxScore = std::min(100.0, (metrics.relaxation / relaxTarget) * 100.0);
            score += relaxScore * targets.relaxation_weight;
            totalWeight += targets.relaxation_weight;
        }

        // Alpha power comparison
        double alphaTarget = (targets.alpha_min + targets.alpha_max) / 2.0;
        if (alphaTarget > 0) {
            double alphaScore = std::min(100.0, (metrics.alpha_power / alphaTarget) * 100.0);
            score += alphaScore * targets.alpha_weight;
            totalWeight += targets.alpha_weight;
        }

        // Theta power comparison - for meditative states
        double thetaTarget = (targets.theta_min + targets.theta_max) / 2.0;
        if (thetaTarget > 0) {
            double thetaScore = std::min(100.0, (metrics.theta_power / thetaTarget) * 100.0);
            score += thetaScore * targets.theta_weight;
            totalWeight += targets.theta_weight;
        }

        // Fatigue penalty (fixed weight 0.1)
        double fatigueScore = std::max(0.0, 100.0 - metrics.fatigue);
        score += fatigueScore * 0.1;
        totalWeight += 0.1;

        if (totalWeight > 0) {
            return std::min(100.0, std::max(0.0, score / totalWeight));
        }
    }

    // Fallback: simple calculation
    double rate = 0.0;
    rate += metrics.concentration * 0.3;
    rate += metrics.relaxation * 0.2;
    rate += (100.0 - metrics.fatigue) * 0.2;
    rate += std::min(metrics.alpha_power, 20.0) * 1.5;  // Cap alpha at 20, scale to ~30%

    return std::min(100.0, std::max(0.0, rate));
}

void ExerciseController::setDuration(int minutes)
{
    if (m_duration != minutes && minutes > 0) {
        m_duration = minutes;

        // Если упражнение уже активно, обновляем его длительность
        if (m_currentExercise) {
            m_currentExercise->setDuration(minutes);
            m_totalSeconds = minutes * 60;
            emit totalSecondsChanged();
        }

        emit durationChanged();
        qDebug() << "[ExerciseController] Длительность установлена:" << minutes << "минут";
    }
}

void ExerciseController::setRecommendedDuration(int practiceLevel, bool useMax)
{
    // Диапазоны длительности для каждого уровня
    // 0 = Beginner: 1-2 минуты
    // 1 = Intermediate: 2-5 минут
    // 2 = Expert: 5-10 минут

    int minDuration, maxDuration;

    switch (practiceLevel) {
        case 0: // Beginner
            minDuration = 1;
            maxDuration = 2;
            break;
        case 1: // Intermediate
            minDuration = 2;
            maxDuration = 5;
            break;
        case 2: // Expert
            minDuration = 5;
            maxDuration = 10;
            break;
        default:
            minDuration = 1;
            maxDuration = 2;
    }

    int duration = useMax ? maxDuration : minDuration;
    setDuration(duration);

    qDebug() << "[ExerciseController] Рекомендуемая длительность для уровня" << practiceLevel
             << ":" << duration << "минут";
}
