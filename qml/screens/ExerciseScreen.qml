import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"
import "../components"

Item {
    id: exerciseScreen

    signal exerciseCompleted()
    signal back()

    // Имя упражнения (передается из PreparationScreen)
    property string exerciseName: ""

    // ДОБАВЛЕНО: Связь с моделью метрик
    property var metricsModel: mainWindow ? mainWindow.appMetricsModel : null

    // Остановка упражнения при уходе с экрана
    Component.onDestruction: {
        if (exerciseController && exerciseController.isActive) {
            exerciseController.stopExercise()
        }
    }

    // Состояние упражнения
    property int elapsedSeconds: 0
    // ИСПРАВЛЕНО: Берём длительность из exerciseController (в минутах -> секунды)
    property int totalSeconds: exerciseController ? exerciseController.duration * 60 : 300
    property bool isRunning: true
    property bool isPaused: false
    property int currentPhase: 0  // 0 = Подготовка, 1 = Основная, 2 = Завершение

    // НОВОЕ: Снимки метрик для каждой фазы (для ResultsScreen)
    property var baselineSnapshot: null
    property var activeSnapshot: null
    property var postSnapshot: null

    // ИСПРАВЛЕНО: Фазы рассчитываются динамически на основе totalSeconds
    // Baseline и Post по 30 сек, остальное - практика
    property int baselineDuration: 30
    property int postDuration: 30
    property int practiceDuration: Math.max(totalSeconds - baselineDuration - postDuration, 60)

    property var phases: [
        { name: "📊 Baseline", description: "Запись базовых метрик", duration: baselineDuration, icon: "📊", color: Theme.infoColor },
        { name: "⚡ Практика", description: "Основное выполнение", duration: practiceDuration, icon: "⚡", color: Theme.primaryColor },
        { name: "📊 Post", description: "Запись финальных метрик", duration: postDuration, icon: "📊", color: Theme.successColor }
    ]

    // Инструкции для каждой фазы
    property var instructions: [
        "Закройте глаза. Расслабьтесь. Дышите спокойно. Сейчас записываются ваши базовые показатели мозга в состоянии покоя.",
        "Медленно сближайте и отдаляйте ладони, чувствуя пространство между ними. Представьте упругий шар энергии.",
        "Сохраните ощущение энергии. Продолжайте спокойно дышать. Записываются финальные показатели для сравнения."
    ]

    // Рассчитываемые свойства
    property int phaseElapsed: {
        var elapsed = elapsedSeconds
        for (var i = 0; i < currentPhase; i++) {
            elapsed -= phases[i].duration
        }
        return elapsed
    }

    property int phaseDuration: currentPhase < phases.length ? phases[currentPhase].duration : 30
    property string phaseName: currentPhase < phases.length ? phases[currentPhase].name : "✅ Завершено"
    property string phaseDescription: currentPhase < phases.length ? phases[currentPhase].description : "Упражнение завершено"
    property string phaseIcon: currentPhase < phases.length ? phases[currentPhase].icon : "✅"
    property color phaseColor: currentPhase < phases.length ? phases[currentPhase].color : Theme.successColor
    property string currentInstruction: currentPhase < instructions.length ? instructions[currentPhase] : ""

    // Форматирование времени MM:SS
    function formatTime(seconds) {
        var mins = Math.floor(seconds / 60)
        var secs = seconds % 60
        return (mins < 10 ? "0" : "") + mins + ":" + (secs < 10 ? "0" : "") + secs
    }

    // НОВОЕ: Функция для создания снимка текущих метрик
    function captureMetricsSnapshot() {
        if (!metricsModel) return null

        return {
            "alpha": metricsModel.alpha,
            "beta": metricsModel.beta,
            "theta": metricsModel.theta,
            "concentration": metricsModel.concentration,
            "relaxation": metricsModel.relaxation,
            "fatigue": metricsModel.fatigue,
            "focus": metricsModel.focus,
            "stress": metricsModel.stress,
            "heartRate": metricsModel.heartRate,
            "successRate": metricsModel.successRate
        }
    }

    // Таймер обновления
    Timer {
        id: exerciseTimer
        interval: 1000
        running: exerciseScreen.isRunning && !exerciseScreen.isPaused
        repeat: true

        onTriggered: {
            elapsedSeconds++

            // Проверка смены фазы
            var phaseEnd = 0
            for (var i = 0; i <= currentPhase && i < phases.length; i++) {
                phaseEnd += phases[i].duration
            }

            if (elapsedSeconds >= phaseEnd && currentPhase < phases.length - 1) {
                // НОВОЕ: Сохранить снимок метрик при завершении фазы
                if (currentPhase === 0) {
                    // Завершена Baseline фаза
                    baselineSnapshot = captureMetricsSnapshot()
                    console.log("📊 Baseline snapshot сохранен:", JSON.stringify(baselineSnapshot))
                } else if (currentPhase === 1) {
                    // Завершена Active фаза
                    activeSnapshot = captureMetricsSnapshot()
                    console.log("⚡ Active snapshot сохранен:", JSON.stringify(activeSnapshot))
                }

                currentPhase++
                console.log("Переход в фазу:", currentPhase, phaseName)
            }

            // Завершение упражнения
            if (elapsedSeconds >= totalSeconds) {
                // НОВОЕ: Сохранить Post snapshot перед завершением
                postSnapshot = captureMetricsSnapshot()
                console.log("📊 Post snapshot сохранен:", JSON.stringify(postSnapshot))

                exerciseTimer.stop()
                isRunning = false
                // Автоматически перейти к результатам через 2 секунды
                Qt.callLater(function() {
                    finishTimer.start()
                })
            }
        }
    }

    // Таймер для автоматического завершения
    Timer {
        id: finishTimer
        interval: 2000
        running: false
        repeat: false
        onTriggered: exerciseCompleted()
    }

    Component.onCompleted: {
        // Запуск упражнения через ExerciseController
        if (exerciseName && exerciseController) {
            console.log("[ExerciseScreen] Запуск упражнения:", exerciseName)
            exerciseController.startExercise(exerciseName)
        }
        console.log("ExerciseScreen запущен. Длительность:", totalSeconds, "сек")
        exerciseTimer.start()
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingLarge

            // Левая панель - Таймер и фаза
            Rectangle {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingMedium

                    // Название упражнения
                    Text {
                        Layout.fillWidth: true
                        text: exerciseScreen.exerciseName || "Упражнение"
                        font.pixelSize: Theme.fontSizeHeading2
                        font.weight: Theme.fontWeightBold
                        color: Theme.adaptiveTextPrimary
                        wrapMode: Text.WordWrap
                    }

                    Item { height: Theme.paddingLarge }

                    // Фаза упражнения (трехфазная методология Бронникова)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 140
                        color: Qt.rgba(phaseColor.r, phaseColor.g, phaseColor.b, 0.15)
                        radius: Theme.radiusMedium
                        border.color: phaseColor
                        border.width: 1

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: phaseIcon
                                font.pixelSize: 40
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: "Фаза " + (currentPhase + 1) + " / " + phases.length
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.adaptiveTextPrimary
                                font.weight: Theme.fontWeightMedium
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: phaseName
                                font.pixelSize: Theme.fontSizeHeading2
                                font.weight: Theme.fontWeightBold
                                color: Theme.adaptiveTextPrimary
                            }

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: phaseDescription
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.adaptiveTextSecondary
                            }
                        }
                    }

                    Item { height: Theme.paddingMedium }

                    // Таймер - общее время
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: formatTime(elapsedSeconds)
                        font.pixelSize: 48
                        font.weight: Theme.fontWeightBold
                        font.family: Theme.fontFamilyMono
                        color: isPaused ? Theme.warningColor : Theme.adaptiveTextPrimary
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "/ " + formatTime(totalSeconds)
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    // Таймер фазы
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Фаза: " + formatTime(phaseElapsed) + " / " + formatTime(phaseDuration)
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.adaptiveTextSecondary
                        font.family: Theme.fontFamilyMono
                    }

                    Item { height: Theme.paddingSmall }

                    // Прогресс-бар общего времени
                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: totalSeconds
                        value: elapsedSeconds
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: Math.round((elapsedSeconds / totalSeconds) * 100) + "%"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    // ОБНОВЛЕНО: Прогресс-бар текущей фазы с цветом фазы
                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: phaseDuration
                        value: phaseElapsed

                        background: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 6
                            color: Theme.backgroundColor
                            radius: 3
                        }

                        contentItem: Item {
                            implicitWidth: 200
                            implicitHeight: 6

                            Rectangle {
                                width: parent.width * (phaseElapsed / phaseDuration)
                                height: parent.height
                                radius: 3
                                color: phaseColor
                            }
                        }
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Фаза: " + Math.round((phaseElapsed / phaseDuration) * 100) + "%"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.adaptiveTextSecondary
                    }

                    Item { Layout.fillHeight: true }

                    // Кнопки управления
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Theme.paddingMedium

                        Button {
                            Layout.fillWidth: true
                            text: isPaused ? "▶️ Продолжить" : "⏸️ Пауза"
                            font.pixelSize: isPaused ? Theme.fontSizeBody : 20

                            background: Rectangle {
                                color: parent.down ? Qt.darker(isPaused ? Theme.successColor : Theme.warningColor, 1.1) :
                                       parent.hovered ? Qt.lighter(isPaused ? Theme.successColor : Theme.warningColor, 1.1) :
                                       (isPaused ? Theme.successColor : Theme.warningColor)
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
                                isPaused = !isPaused
                                console.log(isPaused ? "⏸️ Пауза" : "▶️ Возобновлено")
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "⏹️ Стоп"
                            font.pixelSize: Theme.fontSizeBody

                            background: Rectangle {
                                color: parent.down ? Qt.darker(Theme.errorColor, 1.1) :
                                       parent.hovered ? Qt.lighter(Theme.errorColor, 1.1) :
                                       Theme.errorColor
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
                                exerciseTimer.stop()
                                isRunning = false
                                exerciseCompleted()
                            }
                        }
                    }
                }
            }

            // Правая панель - 3 строки: График, Метрики, Инструкции
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Theme.paddingSmall

                // 1. График Alpha/Beta/Theta (вверху)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 200
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingSmall
                        spacing: 4

                        Text {
                            text: "Real-time Alpha/Beta/Theta"
                            font.pixelSize: Theme.fontSizeBody
                            font.weight: Theme.fontWeightMedium
                            color: Theme.adaptiveTextPrimary
                        }

                        LineChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            alphaHistory: metricsModel ? metricsModel.alphaHistory : []
                            betaHistory: metricsModel ? metricsModel.betaHistory : []
                            thetaHistory: metricsModel ? metricsModel.thetaHistory : []
                        }
                    }
                }

                // 2. Метрики (посередине)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 150
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingSmall
                        spacing: Theme.paddingMedium

                        // Колонка 1: Alpha/Beta/Theta
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Text { text: "Волны"; font.pixelSize: 14; font.weight: Font.Medium; color: Theme.adaptiveTextPrimary }

                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Alpha"; color: Theme.alphaColor; font.pixelSize: 13; Layout.preferredWidth: 50 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.alpha : 0; customColor: Theme.alphaColor }
                                Text { text: metricsModel ? Math.round(metricsModel.alpha) + "%" : "0%"; font.family: Theme.fontFamilyMono; font.pixelSize: 13; color: Theme.alphaColor; Layout.preferredWidth: 40 }
                            }
                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Beta"; color: Theme.betaColor; font.pixelSize: 13; Layout.preferredWidth: 50 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.beta : 0; customColor: Theme.betaColor }
                                Text { text: metricsModel ? Math.round(metricsModel.beta) + "%" : "0%"; font.family: Theme.fontFamilyMono; font.pixelSize: 13; color: Theme.betaColor; Layout.preferredWidth: 40 }
                            }
                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Theta"; color: Theme.thetaColor; font.pixelSize: 13; Layout.preferredWidth: 50 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.theta : 0; customColor: Theme.thetaColor }
                                Text { text: metricsModel ? Math.round(metricsModel.theta) + "%" : "0%"; font.family: Theme.fontFamilyMono; font.pixelSize: 13; color: Theme.thetaColor; Layout.preferredWidth: 40 }
                            }
                        }

                        Rectangle { width: 1; Layout.fillHeight: true; color: Theme.borderColor; opacity: 0.3 }

                        // Колонка 2: Концентрация/Релаксация
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Text { text: "Состояние"; font.pixelSize: 14; font.weight: Font.Medium; color: Theme.adaptiveTextPrimary }

                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Концентр."; color: Theme.adaptiveTextSecondary; font.pixelSize: 13; Layout.preferredWidth: 70 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.concentration : 0 }
                                Text { text: metricsModel ? Math.round(metricsModel.concentration) + "%" : "0%"; font.family: Theme.fontFamilyMono; font.pixelSize: 13; color: Theme.concentrationColor; Layout.preferredWidth: 40 }
                            }
                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Релаксация"; color: Theme.adaptiveTextSecondary; font.pixelSize: 13; Layout.preferredWidth: 70 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.relaxation : 0 }
                                Text { text: metricsModel ? Math.round(metricsModel.relaxation) + "%" : "0%"; font.family: Theme.fontFamilyMono; font.pixelSize: 13; color: Theme.relaxationColor; Layout.preferredWidth: 40 }
                            }
                            RowLayout { Layout.fillWidth: true; spacing: 6
                                Text { text: "Success"; color: Theme.adaptiveTextSecondary; font.pixelSize: 13; Layout.preferredWidth: 70 }
                                MetricsProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: metricsModel ? metricsModel.successRate : 0 }
                                Text {
                                    text: metricsModel ? Math.round(metricsModel.successRate) + "%" : "0%"
                                    font.family: Theme.fontFamilyMono; font.pixelSize: 13; font.weight: Font.Bold; Layout.preferredWidth: 40
                                    color: { var r = metricsModel ? metricsModel.successRate : 0; return r >= 70 ? Theme.successColor : r >= 40 ? Theme.warningColor : Theme.errorColor }
                                }
                            }
                        }

                        Rectangle { width: 1; Layout.fillHeight: true; color: Theme.borderColor; opacity: 0.3 }

                        // Колонка 3: Пульс
                        ColumnLayout {
                            Layout.preferredWidth: 100
                            spacing: 4

                            Text { text: "Пульс"; font.pixelSize: 14; font.weight: Font.Medium; color: Theme.adaptiveTextPrimary }

                            Text {
                                text: metricsModel ? metricsModel.heartRate : "0"
                                font.family: Theme.fontFamilyMono
                                font.pixelSize: 40
                                font.weight: Font.Bold
                                color: Theme.heartRateColor
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: "BPM"
                                font.pixelSize: 13
                                color: Theme.adaptiveTextSecondary
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }

                // 3. Инструкции (внизу)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 120
                    color: Theme.surfaceColor
                    border.color: phaseColor
                    border.width: 2
                    radius: Theme.radiusMedium

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingMedium

                        // Иконка фазы
                        Text {
                            text: phaseIcon
                            font.pixelSize: 40
                            Layout.alignment: Qt.AlignVCenter
                        }

                        // Текст инструкции
                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 4

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: Theme.paddingSmall

                                Text {
                                    text: phaseName
                                    font.pixelSize: Theme.fontSizeHeading3
                                    font.weight: Theme.fontWeightMedium
                                    color: phaseColor
                                }

                                Item { Layout.fillWidth: true }

                                // Индикатор паузы
                                Rectangle {
                                    width: 70; height: 24; radius: 12
                                    color: Theme.warningColor
                                    visible: isPaused
                                    Text { anchors.centerIn: parent; text: "ПАУЗА"; font.pixelSize: 12; font.weight: Font.Bold; color: "white" }
                                }

                                // Индикатор озвучивания
                                Row {
                                    spacing: 6
                                    visible: audioController.isSpeaking
                                    Rectangle {
                                        width: 10; height: 10; radius: 5; color: Theme.primaryColor
                                        SequentialAnimation on opacity { running: audioController.isSpeaking; loops: Animation.Infinite
                                            NumberAnimation { from: 1.0; to: 0.3; duration: 500 }
                                            NumberAnimation { from: 0.3; to: 1.0; duration: 500 }
                                        }
                                    }
                                    Text { text: "Озвучивание"; font.pixelSize: 12; color: Theme.adaptiveTextSecondary }
                                }
                            }

                            Text {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                text: currentInstruction
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextPrimary
                                wrapMode: Text.WordWrap
                                verticalAlignment: Text.AlignTop
                            }
                        }
                    }
                }
            }
        }
    }
}
