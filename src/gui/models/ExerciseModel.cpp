#include "models/ExerciseModel.h"
#include "ExerciseLibrary.h"
#include "Exercise.h"
#include <QDebug>

ExerciseModel::ExerciseModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_library(std::make_unique<Bronnikov::ExerciseLibrary>())
{
}

ExerciseModel::~ExerciseModel() = default;

int ExerciseModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_exercises.size());
}

QVariant ExerciseModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_exercises.size()))
        return QVariant();

    const auto& exercise = m_exercises[index.row()];

    switch (role) {
    case NameRole:
        return QString::fromStdString(exercise->getName());
    case DescriptionRole:
        return QString::fromStdString(exercise->getDescription());
    case DurationRole:
        return exercise->getDuration();
    case StageRole:
        return static_cast<int>(exercise->getStage());
    case OrderRole:
        return exercise->getOrderInStage();
    case StageIdRole:
        return QString::fromStdString(exercise->getStageId());
    case IconEmojiRole:
        return getIconForExercise(exercise->getName());
    case IsCompletedRole:
        return isExerciseCompleted(exercise->getName());
    case BestSuccessRateRole:
        return getBestSuccessRate(exercise->getName());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ExerciseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[DurationRole] = "duration";
    roles[StageRole] = "stage";
    roles[OrderRole] = "order";
    roles[StageIdRole] = "stageId";
    roles[IconEmojiRole] = "iconEmoji";
    roles[IsCompletedRole] = "isCompleted";
    roles[BestSuccessRateRole] = "bestSuccessRate";
    return roles;
}

void ExerciseModel::loadExercises()
{
    beginResetModel();
    m_exercises = m_library->getExercises();
    endResetModel();
    emit countChanged();
    emit exercisesLoaded();
}

void ExerciseModel::filterByStage(int stage)
{
    beginResetModel();
    m_currentStage = stage;
    m_exercises = m_library->getExercisesByStage(static_cast<Bronnikov::ExerciseStage>(stage));
    endResetModel();
    emit currentStageChanged();
    emit countChanged();
}

void ExerciseModel::resetFilter()
{
    beginResetModel();
    m_currentStage = 0;
    m_exercises = m_library->getExercises();
    endResetModel();
    emit currentStageChanged();
    emit countChanged();
}

QVariantMap ExerciseModel::getExercise(int index) const
{
    QVariantMap result;
    if (index < 0 || index >= static_cast<int>(m_exercises.size()))
        return result;

    const auto& exercise = m_exercises[index];
    result["name"] = QString::fromStdString(exercise->getName());
    result["description"] = QString::fromStdString(exercise->getDescription());
    result["duration"] = exercise->getDuration();
    result["stage"] = static_cast<int>(exercise->getStage());
    result["order"] = exercise->getOrderInStage();
    result["stageId"] = QString::fromStdString(exercise->getStageId());

    return result;
}

void ExerciseModel::setCurrentStage(int stage)
{
    if (m_currentStage != stage) {
        m_currentStage = stage;
        filterByStage(stage);
    }
}

QString ExerciseModel::getIconForExercise(const std::string& exerciseName) const
{
    // TODO: Map exercise names to icons
    if (exerciseName.find("шар") != std::string::npos) return "⚡";
    if (exerciseName.find("Движение") != std::string::npos) return "🔄";
    if (exerciseName.find("палец") != std::string::npos) return "✋";
    if (exerciseName.find("Дыхание") != std::string::npos) return "🌬️";
    if (exerciseName.find("ветер") != std::string::npos) return "💨";
    if (exerciseName.find("глаз") != std::string::npos) return "👁️";
    if (exerciseName.find("тело") != std::string::npos) return "🌟";
    if (exerciseName.find("Небо") != std::string::npos || exerciseName.find("Земл") != std::string::npos) return "🌍";
    if (exerciseName.find("Тяжел") != std::string::npos || exerciseName.find("Легк") != std::string::npos) return "⚖️";
    if (exerciseName.find("Объем") != std::string::npos) return "🎯";
    return "🧘";
}

bool ExerciseModel::isExerciseCompleted(const std::string& exerciseName) const
{
    // TODO: Query from SessionDatabase
    Q_UNUSED(exerciseName);
    return false;
}

double ExerciseModel::getBestSuccessRate(const std::string& exerciseName) const
{
    // TODO: Query from SessionDatabase
    Q_UNUSED(exerciseName);
    return 0.0;
}
