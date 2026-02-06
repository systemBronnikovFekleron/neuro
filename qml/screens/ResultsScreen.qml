import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    signal returnHome()
    signal repeat()
    signal showStatistics()

    // НОВОЕ: Снимки метрик из ExerciseScreen
    property var baselineSnapshot: null
    property var activeSnapshot: null
    property var postSnapshot: null

    // НОВОЕ: Свойства для трехфазных метрик (вычисляются из снимков)
    property real successRate: postSnapshot ? postSnapshot.successRate : 0.0
    property real effectiveness: {
        if (!baselineSnapshot || !postSnapshot) return 0.0
        // Простая формула effectiveness = улучшение концентрации + снижение стресса
        var concentrationImprovement = postSnapshot.concentration - baselineSnapshot.concentration
        var stressReduction = baselineSnapshot.stress - postSnapshot.stress
        return Math.max(0, Math.min(100, 50 + concentrationImprovement + stressReduction))
    }
    property bool targetAchieved: successRate >= 60.0

    // Baseline метрики
    property real baselineAlpha: baselineSnapshot ? baselineSnapshot.alpha : 0.0
    property real baselineBeta: baselineSnapshot ? baselineSnapshot.beta : 0.0
    property real baselineTheta: baselineSnapshot ? baselineSnapshot.theta : 0.0
    property real baselineConcentration: baselineSnapshot ? baselineSnapshot.concentration : 0.0
    property real baselineRelaxation: baselineSnapshot ? baselineSnapshot.relaxation : 0.0

    // Active метрики (средние)
    property real activeAlpha: activeSnapshot ? activeSnapshot.alpha : 0.0
    property real activeBeta: activeSnapshot ? activeSnapshot.beta : 0.0
    property real activeTheta: activeSnapshot ? activeSnapshot.theta : 0.0
    property real activeConcentration: activeSnapshot ? activeSnapshot.concentration : 0.0
    property real activeRelaxation: activeSnapshot ? activeSnapshot.relaxation : 0.0

    // Post метрики
    property real postAlpha: postSnapshot ? postSnapshot.alpha : 0.0
    property real postBeta: postSnapshot ? postSnapshot.beta : 0.0
    property real postTheta: postSnapshot ? postSnapshot.theta : 0.0
    property real postConcentration: postSnapshot ? postSnapshot.concentration : 0.0
    property real postRelaxation: postSnapshot ? postSnapshot.relaxation : 0.0

    // DEBUG: Логирование при создании
    Component.onCompleted: {
        console.log("📊 ResultsScreen initialized")
        console.log("  Baseline:", JSON.stringify(baselineSnapshot))
        console.log("  Active:", JSON.stringify(activeSnapshot))
        console.log("  Post:", JSON.stringify(postSnapshot))
        console.log("  Success Rate:", successRate)
    }

    // Вспомогательная функция для форматирования изменений
    function formatChange(value) {
        var sign = value >= 0 ? "+" : ""
        return sign + value.toFixed(1) + "%"
    }

    // Цвет изменения (зеленый для положительных, красный для отрицательных)
    function changeColor(value) {
        return value >= 0 ? Theme.successColor : Theme.errorColor
    }

    // Иконка изменения
    function changeIcon(value) {
        if (value > 5) return "↗️"
        if (value < -5) return "↘️"
        return "→"
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        ScrollView {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            contentWidth: availableWidth

            ColumnLayout {
                width: parent.width
                spacing: Theme.paddingLarge

                // Заголовок
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "✓ Сессия завершена"
                    font.pixelSize: Theme.fontSizeDisplay
                    font.weight: Theme.fontWeightBold
                    color: Theme.successColor
                }

                // Общие результаты
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingLarge

                        // Success Rate
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: Theme.paddingSmall

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Success Rate"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: Math.round(successRate) + "%"
                                font.pixelSize: 48
                                font.weight: Theme.fontWeightBold
                                color: successRate >= 70 ? Theme.successColor : Theme.warningColor
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: successRate >= 70 ? "🎯 Цель достигнута!" : "🎯 Цель: 70%"
                                font.pixelSize: Theme.fontSizeSmall
                                color: successRate >= 70 ? Theme.successColor : Theme.warningColor
                            }
                        }

                        Rectangle {
                            width: 2
                            Layout.fillHeight: true
                            color: Theme.backgroundColor
                        }

                        // Overall Effectiveness
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: Theme.paddingSmall

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Эффективность"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: Math.round(effectiveness) + "%"
                                font.pixelSize: 48
                                font.weight: Theme.fontWeightBold
                                color: Theme.primaryColor
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: targetAchieved ? "✅ Целевое состояние" : "⚠️ Близко к цели"
                                font.pixelSize: Theme.fontSizeSmall
                                color: targetAchieved ? Theme.successColor : Theme.warningColor
                            }
                        }
                    }
                }

                Item { height: Theme.paddingSmall }

                // НОВОЕ: Таблица сравнения трех фаз
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 380
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingMedium

                        Text {
                            text: "📊 Трехфазный анализ (методика Бронникова)"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.adaptiveTextPrimary
                        }

                        Text {
                            text: "Сравнение метрик в трех фазах: Baseline (до) → Active (во время) → Post (после)"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        // Заголовки таблицы
                        Rectangle {
                            Layout.fillWidth: true
                            height: 40
                            color: Theme.backgroundColor
                            radius: Theme.radiusSmall

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingSmall
                                spacing: 0

                                Text {
                                    Layout.preferredWidth: 140
                                    text: "Метрика"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: Theme.adaptiveTextPrimary
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: "Baseline"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: Theme.infoColor
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: "Active"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: Theme.primaryColor
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: "Post"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: Theme.successColor
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: "Изменение"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: Theme.adaptiveTextPrimary
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        // Строка: Alpha
                        Item {
                            Layout.fillWidth: true
                            height: 35

                            RowLayout {
                                anchors.fill: parent
                                spacing: 0

                                Text {
                                    Layout.preferredWidth: 140
                                    text: "Alpha"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(baselineAlpha) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(activeAlpha) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(postAlpha) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    font.weight: Theme.fontWeightBold
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: changeIcon((postAlpha - baselineAlpha) / baselineAlpha * 100) + " " +
                                          formatChange((postAlpha - baselineAlpha) / baselineAlpha * 100)
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: changeColor((postAlpha - baselineAlpha) / baselineAlpha * 100)
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        // Строка: Beta
                        Item {
                            Layout.fillWidth: true
                            height: 35

                            RowLayout {
                                anchors.fill: parent
                                spacing: 0

                                Text {
                                    Layout.preferredWidth: 140
                                    text: "Beta"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(baselineBeta) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(activeBeta) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(postBeta) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    font.weight: Theme.fontWeightBold
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: changeIcon((postBeta - baselineBeta) / baselineBeta * 100) + " " +
                                          formatChange((postBeta - baselineBeta) / baselineBeta * 100)
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: changeColor((postBeta - baselineBeta) / baselineBeta * 100)
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        // Строка: Concentration
                        Item {
                            Layout.fillWidth: true
                            height: 35

                            RowLayout {
                                anchors.fill: parent
                                spacing: 0

                                Text {
                                    Layout.preferredWidth: 140
                                    text: "Концентрация"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(baselineConcentration) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(activeConcentration) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(postConcentration) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    font.weight: Theme.fontWeightBold
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: changeIcon((postConcentration - baselineConcentration) / baselineConcentration * 100) + " " +
                                          formatChange((postConcentration - baselineConcentration) / baselineConcentration * 100)
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: changeColor((postConcentration - baselineConcentration) / baselineConcentration * 100)
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        // Строка: Relaxation
                        Item {
                            Layout.fillWidth: true
                            height: 35

                            RowLayout {
                                anchors.fill: parent
                                spacing: 0

                                Text {
                                    Layout.preferredWidth: 140
                                    text: "Релаксация"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(baselineRelaxation) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(activeRelaxation) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.preferredWidth: 100
                                    text: Math.round(postRelaxation) + "%"
                                    font.pixelSize: Theme.fontSizeBody
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                    font.weight: Theme.fontWeightBold
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: changeIcon((postRelaxation - baselineRelaxation) / baselineRelaxation * 100) + " " +
                                          formatChange((postRelaxation - baselineRelaxation) / baselineRelaxation * 100)
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightBold
                                    color: changeColor((postRelaxation - baselineRelaxation) / baselineRelaxation * 100)
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }
                    }
                }

                Item { height: Theme.paddingMedium }

                // Кнопки управления
                Row {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: Theme.paddingMedium

                Button {
                    text: "🔄 Повторить"

                    background: Rectangle {
                        color: parent.down ? Qt.darker(Theme.primaryColor, 1.1) :
                               parent.hovered ? Qt.lighter(Theme.primaryColor, 1.1) :
                               Theme.primaryColor
                        radius: Theme.radiusSmall
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: repeat()
                }

                Button {
                    text: "📊 Статистика"

                    background: Rectangle {
                        color: parent.down ? Qt.darker(Theme.secondaryColor, 1.1) :
                               parent.hovered ? Qt.lighter(Theme.secondaryColor, 1.1) :
                               Theme.secondaryColor
                        radius: Theme.radiusSmall
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        showStatistics()
                    }
                }

                Button {
                    text: "🏠 Главная"

                    background: Rectangle {
                        color: parent.down ? Qt.darker(Theme.successColor, 1.1) :
                               parent.hovered ? Qt.lighter(Theme.successColor, 1.1) :
                               Theme.successColor
                        radius: Theme.radiusSmall
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: returnHome()
                }
            }
        }  // ColumnLayout
    }  // ScrollView
    }  // Rectangle
}
