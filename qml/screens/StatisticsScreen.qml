import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    id: root
    signal back()

    // Data from C++ models
    property int totalSessions: sessionModel ? sessionModel.totalSessions : 0
    property double totalHours: sessionModel ? sessionModel.totalHours : 0.0
    property double avgSuccessRate: sessionModel ? sessionModel.avgSuccessRate : 0.0
    property string firstSessionDate: sessionModel ? sessionModel.firstSessionDate : ""
    property string lastSessionDate: sessionModel ? sessionModel.lastSessionDate : ""
    property var stageProgressData: sessionModel ? sessionModel.stageProgress : []

    // Load data on component creation
    Component.onCompleted: {
        if (sessionModel) {
            sessionModel.loadStatistics()
            sessionModel.loadStageProgress()
            sessionModel.loadRecentSessions(20)
            exercisesStatistics = sessionModel.getExercisesStatistics()
            calendarData = sessionModel.getSessionsByDay(30)
        }
    }

    // Exercise statistics data
    property var exercisesStatistics: []
    property var calendarData: []

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingMedium

            // Header
            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.paddingMedium

                Button {
                    text: "<- Назад"
                    onClicked: root.back()

                    background: Rectangle {
                        color: parent.hovered ? Theme.primaryColorLight : "transparent"
                        radius: Theme.radiusSmall
                        border.color: Theme.primaryColor
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeBody
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Text {
                    text: "Статистика и прогресс"
                    font.pixelSize: Theme.fontSizeHeading1
                    font.weight: Theme.fontWeightBold
                    color: Theme.adaptiveTextPrimary
                    Layout.fillWidth: true
                }
            }

            // TabBar
            TabBar {
                id: tabBar
                Layout.fillWidth: true

                background: Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusSmall
                }

                TabButton {
                    text: "Общее"
                    width: implicitWidth
                    background: Rectangle {
                        color: tabBar.currentIndex === 0 ? Theme.primaryColor : "transparent"
                        radius: Theme.radiusSmall
                    }
                    contentItem: Text {
                        text: parent.text
                        color: tabBar.currentIndex === 0 ? "white" : Theme.adaptiveTextPrimary
                        font.pixelSize: Theme.fontSizeBody
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                TabButton {
                    text: "По упражнениям"
                    width: implicitWidth
                    background: Rectangle {
                        color: tabBar.currentIndex === 1 ? Theme.primaryColor : "transparent"
                        radius: Theme.radiusSmall
                    }
                    contentItem: Text {
                        text: parent.text
                        color: tabBar.currentIndex === 1 ? "white" : Theme.adaptiveTextPrimary
                        font.pixelSize: Theme.fontSizeBody
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                TabButton {
                    text: "По ступеням"
                    width: implicitWidth
                    background: Rectangle {
                        color: tabBar.currentIndex === 2 ? Theme.primaryColor : "transparent"
                        radius: Theme.radiusSmall
                    }
                    contentItem: Text {
                        text: parent.text
                        color: tabBar.currentIndex === 2 ? "white" : Theme.adaptiveTextPrimary
                        font.pixelSize: Theme.fontSizeBody
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                TabButton {
                    text: "Календарь"
                    width: implicitWidth
                    background: Rectangle {
                        color: tabBar.currentIndex === 3 ? Theme.primaryColor : "transparent"
                        radius: Theme.radiusSmall
                    }
                    contentItem: Text {
                        text: parent.text
                        color: tabBar.currentIndex === 3 ? "white" : Theme.adaptiveTextPrimary
                        font.pixelSize: Theme.fontSizeBody
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // Content
            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: tabBar.currentIndex

                // Tab 1: General Statistics
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingLarge

                        Text {
                            text: "Общая статистика"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.adaptiveTextPrimary
                        }

                        // Stats grid
                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            rowSpacing: Theme.paddingMedium
                            columnSpacing: Theme.paddingLarge

                            // Total sessions
                            StatCard {
                                title: "Всего сессий"
                                value: root.totalSessions.toString()
                                icon: "1"
                                Layout.fillWidth: true
                            }

                            // Total hours
                            StatCard {
                                title: "Общее время"
                                value: root.totalHours.toFixed(1) + " ч"
                                icon: "2"
                                Layout.fillWidth: true
                            }

                            // Average success
                            StatCard {
                                title: "Средний успех"
                                value: root.avgSuccessRate.toFixed(0) + "%"
                                icon: "3"
                                color: root.avgSuccessRate >= 70 ? Theme.successColor :
                                       root.avgSuccessRate >= 50 ? Theme.warningColor : Theme.errorColor
                                Layout.fillWidth: true
                            }

                            // Last session
                            StatCard {
                                title: "Последняя сессия"
                                value: root.lastSessionDate ? root.lastSessionDate.substring(0, 10) : "Нет данных"
                                icon: "4"
                                Layout.fillWidth: true
                            }
                        }

                        // Recent sessions section
                        Text {
                            text: "Последние сессии"
                            font.pixelSize: Theme.fontSizeHeading3
                            font.weight: Theme.fontWeightSemiBold
                            color: Theme.adaptiveTextPrimary
                            Layout.topMargin: Theme.paddingMedium
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: Theme.paddingSmall
                            model: sessionModel ? sessionModel.recentSessions : []

                            delegate: Rectangle {
                                width: ListView.view.width
                                height: 50
                                color: index % 2 === 0 ? Theme.surfaceColor : Qt.darker(Theme.surfaceColor, 1.05)
                                radius: Theme.radiusSmall

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: Theme.paddingSmall
                                    spacing: Theme.paddingMedium

                                    Text {
                                        text: modelData.exerciseName || ""
                                        font.pixelSize: Theme.fontSizeBody
                                        color: Theme.adaptiveTextPrimary
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        text: (modelData.successRate || 0).toFixed(0) + "%"
                                        font.pixelSize: Theme.fontSizeBody
                                        font.weight: Theme.fontWeightSemiBold
                                        color: (modelData.successRate || 0) >= 70 ? Theme.successColor :
                                               (modelData.successRate || 0) >= 50 ? Theme.warningColor : Theme.errorColor
                                    }

                                    Text {
                                        text: modelData.timestamp ? modelData.timestamp.substring(0, 10) : ""
                                        font.pixelSize: Theme.fontSizeSmall
                                        color: Theme.adaptiveTextSecondary
                                    }
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Нет данных о сессиях"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                                visible: parent.count === 0
                            }
                        }
                    }
                }

                // Tab 2: By Exercise
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingMedium

                        Text {
                            text: "Статистика по упражнениям"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.adaptiveTextPrimary
                        }

                        // Header row
                        Rectangle {
                            Layout.fillWidth: true
                            height: 40
                            color: Theme.primaryColorLight
                            radius: Theme.radiusSmall

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingSmall
                                spacing: Theme.paddingMedium

                                Text {
                                    text: "Упражнение"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightSemiBold
                                    color: Theme.adaptiveTextPrimary
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 200
                                }

                                Text {
                                    text: "Сессий"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightSemiBold
                                    color: Theme.adaptiveTextPrimary
                                    Layout.preferredWidth: 60
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    text: "Ср. успех"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightSemiBold
                                    color: Theme.adaptiveTextPrimary
                                    Layout.preferredWidth: 80
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    text: "Лучший"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightSemiBold
                                    color: Theme.adaptiveTextPrimary
                                    Layout.preferredWidth: 80
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    text: "Время"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightSemiBold
                                    color: Theme.adaptiveTextPrimary
                                    Layout.preferredWidth: 60
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }

                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 2
                            model: root.exercisesStatistics

                            delegate: Rectangle {
                                width: ListView.view.width
                                height: 45
                                color: index % 2 === 0 ? Theme.surfaceColor : Qt.darker(Theme.surfaceColor, 1.03)
                                radius: Theme.radiusSmall

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: Theme.paddingSmall
                                    spacing: Theme.paddingMedium

                                    Text {
                                        text: modelData.exerciseName || ""
                                        font.pixelSize: Theme.fontSizeBody
                                        color: Theme.adaptiveTextPrimary
                                        Layout.fillWidth: true
                                        Layout.preferredWidth: 200
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        text: (modelData.sessionCount || 0).toString()
                                        font.pixelSize: Theme.fontSizeBody
                                        color: Theme.adaptiveTextPrimary
                                        Layout.preferredWidth: 60
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    Text {
                                        text: (modelData.avgSuccessRate || 0).toFixed(0) + "%"
                                        font.pixelSize: Theme.fontSizeBody
                                        font.weight: Theme.fontWeightSemiBold
                                        color: (modelData.avgSuccessRate || 0) >= 70 ? Theme.successColor :
                                               (modelData.avgSuccessRate || 0) >= 50 ? Theme.warningColor : Theme.errorColor
                                        Layout.preferredWidth: 80
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    Text {
                                        text: (modelData.bestSuccessRate || 0).toFixed(0) + "%"
                                        font.pixelSize: Theme.fontSizeBody
                                        color: Theme.successColor
                                        Layout.preferredWidth: 80
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    Text {
                                        text: (modelData.totalMinutes || 0) + " мин"
                                        font.pixelSize: Theme.fontSizeSmall
                                        color: Theme.adaptiveTextSecondary
                                        Layout.preferredWidth: 60
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Нет данных об упражнениях"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                                visible: parent.count === 0
                            }
                        }
                    }
                }

                // Tab 3: By Stage
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingLarge

                        Text {
                            text: "Прогресс по ступеням"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.adaptiveTextPrimary
                        }

                        Repeater {
                            model: root.stageProgressData

                            Rectangle {
                                Layout.fillWidth: true
                                height: 100
                                color: Qt.darker(Theme.surfaceColor, 1.03)
                                radius: Theme.radiusMedium
                                border.color: Theme.primaryColorLight
                                border.width: 1

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: Theme.paddingMedium
                                    spacing: Theme.paddingSmall

                                    RowLayout {
                                        Layout.fillWidth: true

                                        Text {
                                            text: getStageName(modelData.stage)
                                            font.pixelSize: Theme.fontSizeHeading3
                                            font.weight: Theme.fontWeightSemiBold
                                            color: Theme.adaptiveTextPrimary
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: (modelData.completionPercentage || 0).toFixed(0) + "%"
                                            font.pixelSize: Theme.fontSizeHeading2
                                            font.weight: Theme.fontWeightBold
                                            color: (modelData.completionPercentage || 0) >= 70 ? Theme.successColor :
                                                   (modelData.completionPercentage || 0) >= 30 ? Theme.warningColor : Theme.adaptiveTextSecondary
                                        }
                                    }

                                    // Progress bar
                                    Rectangle {
                                        Layout.fillWidth: true
                                        height: 12
                                        color: Theme.surfaceColor
                                        radius: 6
                                        border.color: Theme.primaryColorLight
                                        border.width: 1

                                        Rectangle {
                                            width: parent.width * Math.min((modelData.completionPercentage || 0) / 100, 1.0)
                                            height: parent.height
                                            radius: 6
                                            color: (modelData.completionPercentage || 0) >= 70 ? Theme.successColor :
                                                   (modelData.completionPercentage || 0) >= 30 ? Theme.warningColor : Theme.primaryColor
                                        }
                                    }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: Theme.paddingLarge

                                        Text {
                                            text: "Упражнений: " + (modelData.completedExercises || 0) + "/" + (modelData.totalExercises || 0)
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.adaptiveTextSecondary
                                        }

                                        Text {
                                            text: "Сессий: " + (modelData.totalSessions || 0)
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.adaptiveTextSecondary
                                        }

                                        Text {
                                            text: "Ср. успех: " + (modelData.avgSuccessRate || 0).toFixed(0) + "%"
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.adaptiveTextSecondary
                                        }
                                    }
                                }
                            }
                        }

                        Item { Layout.fillHeight: true }

                        Text {
                            text: root.stageProgressData.length === 0 ? "Нет данных о прогрессе по ступеням" : ""
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.adaptiveTextSecondary
                            Layout.alignment: Qt.AlignHCenter
                            visible: root.stageProgressData.length === 0
                        }
                    }
                }

                // Tab 4: Calendar
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingMedium

                        Text {
                            text: "Календарь активности (последние 30 дней)"
                            font.pixelSize: Theme.fontSizeHeading2
                            font.weight: Theme.fontWeightBold
                            color: Theme.adaptiveTextPrimary
                        }

                        // Activity grid
                        GridLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 7
                            rowSpacing: Theme.paddingSmall
                            columnSpacing: Theme.paddingSmall

                            Repeater {
                                model: generateCalendarModel()

                                Rectangle {
                                    width: 40
                                    height: 40
                                    radius: Theme.radiusSmall
                                    color: modelData.count > 0 ?
                                           Qt.rgba(Theme.successColor.r, Theme.successColor.g, Theme.successColor.b,
                                                   Math.min(0.3 + modelData.count * 0.2, 1.0)) :
                                           Qt.darker(Theme.surfaceColor, 1.05)
                                    border.color: modelData.isToday ? Theme.primaryColor : "transparent"
                                    border.width: modelData.isToday ? 2 : 0

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 2

                                        Text {
                                            text: modelData.day
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: modelData.count > 0 ? "white" : Theme.adaptiveTextSecondary
                                            Layout.alignment: Qt.AlignHCenter
                                        }

                                        Text {
                                            text: modelData.count > 0 ? modelData.count.toString() : ""
                                            font.pixelSize: 10
                                            color: "white"
                                            Layout.alignment: Qt.AlignHCenter
                                            visible: modelData.count > 0
                                        }
                                    }

                                    ToolTip.visible: hovered && modelData.date
                                    ToolTip.text: modelData.date + ": " + modelData.count + " сессий"

                                    property bool hovered: mouseArea.containsMouse

                                    MouseArea {
                                        id: mouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                    }
                                }
                            }
                        }

                        // Legend
                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: Theme.paddingMedium

                            Text {
                                text: "Меньше"
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.adaptiveTextSecondary
                            }

                            Repeater {
                                model: [0.3, 0.5, 0.7, 1.0]

                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 2
                                    color: Qt.rgba(Theme.successColor.r, Theme.successColor.g, Theme.successColor.b, modelData)
                                }
                            }

                            Text {
                                text: "Больше"
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.adaptiveTextSecondary
                            }
                        }
                    }
                }
            }
        }
    }

    // Helper component for stat cards
    component StatCard: Rectangle {
        property string title: ""
        property string value: ""
        property string icon: ""
        property color color: Theme.primaryColor

        height: 80
        color: Qt.darker(Theme.surfaceColor, 1.03)
        radius: Theme.radiusMedium
        border.color: Theme.primaryColorLight
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingMedium
            spacing: Theme.paddingSmall

            Text {
                text: title
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.adaptiveTextSecondary
            }

            Text {
                text: value
                font.pixelSize: Theme.fontSizeHeading2
                font.weight: Theme.fontWeightBold
                color: parent.parent.color
            }
        }
    }

    // Helper function for stage names
    function getStageName(stage) {
        switch (stage) {
            case 0: return "Подготовительная ступень"
            case 1: return "1-я ступень: Экология духа"
            case 2: return "2-я ступень: Зрение вне глаз"
            case 3: return "3-я ступень: Экран ЛБК"
            case 4: return "4-я ступень: Радарное видение"
            default: return "Ступень " + stage
        }
    }

    // Generate calendar model for last 30 days
    function generateCalendarModel() {
        var result = []
        var today = new Date()

        // Create a map from calendarData
        var sessionMap = {}
        for (var i = 0; i < root.calendarData.length; i++) {
            sessionMap[root.calendarData[i].date] = root.calendarData[i].sessionCount
        }

        // Generate 30 days
        for (var d = 29; d >= 0; d--) {
            var date = new Date(today)
            date.setDate(date.getDate() - d)

            var dateStr = date.toISOString().substring(0, 10)
            var count = sessionMap[dateStr] || 0

            result.push({
                day: date.getDate(),
                date: dateStr,
                count: count,
                isToday: d === 0
            })
        }

        return result
    }
}
