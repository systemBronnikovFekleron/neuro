import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: homeScreen

    signal exerciseSelected(int exerciseIndex)
    signal showStatistics()
    signal stageChanged(int stage)

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        // Главный контент
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingLarge

            // Header
            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.paddingMedium

                // Логотип и название
                Text {
                    text: "☰ Bronnikov Method"
                    font.pixelSize: Theme.fontSizeHeading1
                    font.weight: Theme.fontWeightBold
                    color: Theme.adaptiveTextPrimary
                }

                Item { Layout.fillWidth: true }

                // Кнопки действий
                Row {
                    spacing: Theme.paddingMedium

                    Button {
                        text: "⚙️ Настройки"
                        onClicked: {
                            // TODO: Открыть настройки
                        }
                    }

                    Button {
                        text: "👤 Профиль"
                        onClicked: {
                            // TODO: Открыть профиль
                        }
                    }

                    Button {
                        text: "📊 Статистика"
                        highlighted: true
                        onClicked: {
                            showStatistics()
                        }
                    }
                }
            }

            // Прогресс по текущей ступени
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 140
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingLarge

                    // Левая часть - информация
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: Theme.paddingSmall

                        Text {
                            text: "📊 Ваш прогресс"
                            font.pixelSize: Theme.fontSizeHeading3
                            font.weight: Theme.fontWeightMedium
                            color: Theme.adaptiveTextPrimary
                        }

                        Text {
                            text: "Подготовительная ступень"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.primaryColor
                        }

                        // Прогресс-бар с использованием CustomProgressBar
                        CustomProgressBar {
                            Layout.fillWidth: true
                            value: sessionModel ? sessionModel.stageProgress[0].completionPercentage : 80
                            progressColor: Theme.primaryColor
                            showLabel: false
                            barHeight: 12
                        }

                        RowLayout {
                            spacing: Theme.paddingMedium

                            Text {
                                text: (sessionModel ? sessionModel.stageProgress[0].completedExercises : 8) +
                                      " / " +
                                      (sessionModel ? sessionModel.stageProgress[0].totalExercises : 10) +
                                      " упражнений освоено"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                text: "•"
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                text: (sessionModel ? sessionModel.totalSessions : 15) + " сессий"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                text: "•"
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                text: "Средний успех: " +
                                      (sessionModel ? sessionModel.avgSuccessRate.toFixed(0) : "72") + "%"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.successColor
                                font.weight: Theme.fontWeightMedium
                            }
                        }
                    }

                    // Правая часть - иконка достижения
                    Rectangle {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 100
                        radius: 50
                        color: Theme.primaryColor
                        opacity: 0.1

                        Text {
                            anchors.centerIn: parent
                            text: "🏆"
                            font.pixelSize: 48
                        }
                    }
                }
            }

            // Селектор ступени с использованием StageSelector
            StageSelector {
                Layout.fillWidth: true
                Layout.preferredHeight: 100

                currentStage: 0
                stageProgress: sessionModel ? [
                    sessionModel.stageProgress[0].completionPercentage,
                    sessionModel.stageProgress[1] ? sessionModel.stageProgress[1].completionPercentage : 0,
                    sessionModel.stageProgress[2] ? sessionModel.stageProgress[2].completionPercentage : 0,
                    sessionModel.stageProgress[3] ? sessionModel.stageProgress[3].completionPercentage : 0
                ] : [80, 0, 0, 0]

                stageUnlocked: [true, false, false, false]  // TODO: логика разблокировки

                onStageSelected: function(stage) {
                    homeScreen.stageChanged(stage)
                    exerciseModel.filterByStage(stage)
                }
            }

            // Заголовок секции упражнений
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "🎯 Выберите упражнение"
                    font.pixelSize: Theme.fontSizeHeading2
                    font.weight: Theme.fontWeightMedium
                    color: Theme.adaptiveTextPrimary
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: exerciseModel ? exerciseModel.count + " упражнений" : "10 упражнений"
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.adaptiveTextSecondary
                }
            }

            // Сетка упражнений с ExerciseCard
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                GridLayout {
                    width: parent.width
                    columns: 4
                    rowSpacing: Theme.paddingMedium
                    columnSpacing: Theme.paddingMedium

                    // Repeater с ExerciseCard компонентом
                    Repeater {
                        model: exerciseModel ? exerciseModel.count : 10

                        delegate: ExerciseCard {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 160

                            // Данные из модели (если доступна) или fallback
                            exerciseName: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.NameRole) :
                                [
                                    "Энергетический шар",
                                    "Движение шара",
                                    "Лучи из пальцев",
                                    "Дыхание мыслью",
                                    "Быстрый ветер",
                                    "Лучи из глаз",
                                    "Тонкое тело",
                                    "Небо и Земля",
                                    "Тяжелое и Легкое",
                                    "Объемное восприятие"
                                ][index]

                            exerciseId: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.StageIdRole) :
                                "0." + (index + 1)

                            duration: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.DurationRole) :
                                [10, 12, 8, 15, 10, 12, 15, 15, 12, 25][index]

                            iconEmoji: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.IconEmojiRole) :
                                ["⚡", "🔄", "✋", "🌬️", "💨", "👁️", "🌟", "🌍", "⚖️", "🎯"][index]

                            isCompleted: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.IsCompletedRole) :
                                false

                            successRate: exerciseModel ?
                                exerciseModel.data(exerciseModel.index(index, 0), exerciseModel.BestSuccessRateRole) :
                                0

                            onClicked: {
                                homeScreen.exerciseSelected(index)
                            }
                        }
                    }
                }
            }
        }
    }
}
