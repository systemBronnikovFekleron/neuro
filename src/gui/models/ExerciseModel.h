#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QVariant>
#include <memory>
#include <vector>

// Forward declarations
namespace Bronnikov {
    class Exercise;
    class ExerciseLibrary;
}

/**
 * @brief Qt Model для отображения списка упражнений в QML
 *
 * Предоставляет доступ к библиотеке упражнений Бронникова через Qt Model/View архитектуру.
 * Используется в HomeScreen для отображения карточек упражнений.
 */
class ExerciseModel : public QAbstractListModel
{
    Q_OBJECT

    // Properties для QML
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int currentStage READ currentStage WRITE setCurrentStage NOTIFY currentStageChanged)

public:
    enum ExerciseRoles {
        NameRole = Qt::UserRole + 1,
        DescriptionRole,
        DurationRole,
        StageRole,
        OrderRole,
        StageIdRole,
        IconEmojiRole,
        IsCompletedRole,
        BestSuccessRateRole
    };

    explicit ExerciseModel(QObject* parent = nullptr);
    ~ExerciseModel() override;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Q_INVOKABLE methods для вызова из QML
    Q_INVOKABLE void loadExercises();
    Q_INVOKABLE void filterByStage(int stage);
    Q_INVOKABLE void resetFilter();
    Q_INVOKABLE QVariantMap getExercise(int index) const;

    // Property getters/setters
    int currentStage() const { return m_currentStage; }
    void setCurrentStage(int stage);

signals:
    void countChanged();
    void currentStageChanged();
    void exercisesLoaded();

private:
    std::unique_ptr<Bronnikov::ExerciseLibrary> m_library;
    std::vector<std::shared_ptr<Bronnikov::Exercise>> m_exercises;
    int m_currentStage = 0;  // 0 = Preparatory

    void updateExercisesList();
    QString getIconForExercise(const std::string& exerciseName) const;
    bool isExerciseCompleted(const std::string& exerciseName) const;
    double getBestSuccessRate(const std::string& exerciseName) const;
};
