import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: exerciseScreen

    signal exerciseCompleted()
    signal back()
    signal pauseToggled()

    property bool isPaused: false

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingLarge

            // Левая панель - Таймер и фаза
            Rectangle {
                Layout.preferredWidth: 320
                Layout.fillHeight: true
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingMedium

                    // Header с кнопкой назад
                    RowLayout {
                        Layout.fillWidth: true

                        Button {
                            text: "← Назад"
                            onClicked: back()
                        }

                        Item { Layout.fillWidth: true }
                    }

                    // Название упражнения
                    Text {
                        Layout.fillWidth: true
                        text: exerciseController ? exerciseController.exerciseName : "⚡ Энергетический шар"
                        font.pixelSize: Theme.fontSizeHeading2
                        font.weight: Theme.fontWeightBold
                        color: Theme.adaptiveTextPrimary
                        wrapMode: Text.WordWrap
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Theme.dividerColor
                    }

                    // Фаза упражнения
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 100
                        color: Theme.primaryColor
                        opacity: 0.15
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: Theme.paddingSmall

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: exerciseController ? exerciseController.currentPhase : "Фаза 2"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightBold
                                color: Theme.primaryColor
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Основная фаза"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.primaryColor
                            }
                        }
                    }

                    Item { height: Theme.paddingMedium }

                    // Таймер
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: {
                            var elapsed = exerciseController ? exerciseController.elapsedSeconds : 342
                            var minutes = Math.floor(elapsed / 60)
                            var seconds = elapsed % 60
                            return (minutes < 10 ? "0" : "") + minutes + ":" +
                                   (seconds < 10 ? "0" : "") + seconds
                        }
                        font.pixelSize: 56
                        font.weight: Theme.fontWeightBold
                        font.family: Theme.fontFamilyMono
                        color: Theme.adaptiveTextPrimary
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: {
                            var total = exerciseController ? exerciseController.totalSeconds : 600
                            var minutes = Math.floor(total / 60)
                            return "/ " + minutes + ":00"
                        }
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    Item { height: Theme.paddingMedium }

                    // Прогресс-бар
                    CustomProgressBar {
                        Layout.fillWidth: true
                        value: exerciseController ? exerciseController.progressPercentage : 57
                        progressColor: Theme.primaryColor
                        barHeight: 10
                        showLabel: true
                        labelSuffix: "%"
                    }

                    Item { Layout.fillHeight: true }

                    // Success Rate индикатор
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 80
                        radius: Theme.radiusSmall
                        color: {
                            var rate = exerciseController ? exerciseController.currentSuccessRate : 68
                            return rate >= 70 ? Theme.successColor : Theme.warningColor
                        }
                        opacity: 0.1

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: Theme.paddingSmall

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Success Rate"
                                font.pixelSize: Theme.fontSizeCaption
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: {
                                    var rate = exerciseController ? exerciseController.currentSuccessRate : 68
                                    return rate.toFixed(0) + "%"
                                }
                                font.pixelSize: Theme.fontSizeDisplay
                                font.family: Theme.fontFamilyMono
                                font.weight: Theme.fontWeightBold
                                color: {
                                    var rate = exerciseController ? exerciseController.currentSuccessRate : 68
                                    return rate >= 70 ? Theme.successColor : Theme.warningColor
                                }
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: {
                                    var target = exerciseController ? exerciseController.targetSuccessRate : 70
                                    return "Цель: " + target + "%"
                                }
                                font.pixelSize: Theme.fontSizeCaption
                                color: Theme.adaptiveTextSecondary
                            }
                        }
                    }

                    // Кнопки управления
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Theme.paddingMedium

                        Button {
                            Layout.fillWidth: true
                            text: exerciseScreen.isPaused ? "▶️ Продолжить" : "⏸️ Пауза"
                            highlighted: !exerciseScreen.isPaused
                            onClicked: {
                                exerciseScreen.isPaused = !exerciseScreen.isPaused
                                pauseToggled()
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "⏹️ Стоп"
                            onClicked: {
                                exerciseCompleted()
                            }
                        }
                    }
                }
            }

            // Правая панель - Метрики и инструкции
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Theme.paddingMedium

                // Панель метрик с использованием MetricsPanel
                MetricsPanel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 280

                    // Связь с моделью метрик
                    alphaValue: metricsModel ? metricsModel.alpha : 68
                    betaValue: metricsModel ? metricsModel.beta : 45
                    thetaValue: metricsModel ? metricsModel.theta : 32
                    concentrationValue: metricsModel ? metricsModel.concentration : 72
                    relaxationValue: metricsModel ? metricsModel.relaxation : 58
                    fatigueValue: metricsModel ? metricsModel.fatigue : 25
                    heartRate: metricsModel ? metricsModel.heartRate : 72
                    successRate: exerciseController ? exerciseController.currentSuccessRate : 68
                }

                // Инструкции
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 140
                    color: Theme.infoColor
                    opacity: 0.1
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingSmall

                        Text {
                            text: "📝 Текущая инструкция:"
                            font.pixelSize: Theme.fontSizeHeading3
                            font.weight: Theme.fontWeightMedium
                            color: Theme.adaptiveTextPrimary
                        }

                        Text {
                            Layout.fillWidth: true
                            text: exerciseController ? exerciseController.currentInstruction :
                                  "Медленно сближайте и отдаляйте ладони, чувствуя пространство между ними. " +
                                  "Представьте упругий шар энергии."
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.adaptiveTextSecondary
                            wrapMode: Text.WordWrap
                        }
                    }
                }

                // График (заглушка для QtCharts)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingMedium

                        // Header графика
                        RowLayout {
                            Layout.fillWidth: true

                            Text {
                                text: "📈 Real-time EEG"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: Theme.adaptiveTextPrimary
                            }

                            Item { Layout.fillWidth: true }

                            // Легенда
                            Row {
                                spacing: Theme.paddingMedium

                                Row {
                                    spacing: Theme.paddingSmall
                                    Rectangle {
                                        width: 12; height: 12
                                        radius: 2
                                        color: Theme.alphaColor
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Text {
                                        text: "Alpha"
                                        font.pixelSize: Theme.fontSizeCaption
                                        color: Theme.adaptiveTextSecondary
                                    }
                                }

                                Row {
                                    spacing: Theme.paddingSmall
                                    Rectangle {
                                        width: 12; height: 12
                                        radius: 2
                                        color: Theme.betaColor
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Text {
                                        text: "Beta"
                                        font.pixelSize: Theme.fontSizeCaption
                                        color: Theme.adaptiveTextSecondary
                                    }
                                }

                                Row {
                                    spacing: Theme.paddingSmall
                                    Rectangle {
                                        width: 12; height: 12
                                        radius: 2
                                        color: Theme.thetaColor
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Text {
                                        text: "Theta"
                                        font.pixelSize: Theme.fontSizeCaption
                                        color: Theme.adaptiveTextSecondary
                                    }
                                }
                            }
                        }

                        // Placeholder для графика
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: Theme.backgroundColor
                            radius: Theme.radiusSmall

                            Text {
                                anchors.centerIn: parent
                                text: "График будет здесь\n(QtCharts интеграция)"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                                horizontalAlignment: Text.AlignHCenter
                            }

                            // TODO: Заменить на QtCharts LineSeries
                            // ChartView {
                            //     anchors.fill: parent
                            //     antialiasing: true
                            //
                            //     ValueAxis {
                            //         id: axisX
                            //         min: 0; max: 120
                            //     }
                            //     ValueAxis {
                            //         id: axisY
                            //         min: 0; max: 100
                            //     }
                            //
                            //     LineSeries {
                            //         name: "Alpha"
                            //         axisX: axisX
                            //         axisY: axisY
                            //         color: Theme.alphaColor
                            //         // data from metricsModel.alphaHistory
                            //     }
                            // }
                        }
                    }
                }
            }
        }
    }
}
