import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15
import "../styles"

Item {
    id: exerciseScreen

    signal exerciseCompleted()
    signal back()

    // ДОБАВЛЕНО: Связь с моделью метрик
    property var metricsModel: mainWindow ? mainWindow.appMetricsModel : null

    // Состояние упражнения
    property int elapsedSeconds: 0
    property int totalSeconds: 600  // 10 минут по умолчанию
    property bool isRunning: true
    property bool isPaused: false
    property int currentPhase: 0  // 0 = Подготовка, 1 = Основная, 2 = Завершение

    // НОВОЕ: Снимки метрик для каждой фазы (для ResultsScreen)
    property var baselineSnapshot: null
    property var activeSnapshot: null
    property var postSnapshot: null

    // НОВОЕ: Фазы упражнения по методике Бронникова (трехфазный сбор метрик)
    property var phases: [
        { name: "📊 Baseline", description: "Запись базовых метрик", duration: 30, icon: "📊", color: Theme.infoColor },
        { name: "⚡ Практика", description: "Основное выполнение", duration: 540, icon: "⚡", color: Theme.primaryColor },
        { name: "📊 Post", description: "Запись финальных метрик", duration: 30, icon: "📊", color: Theme.successColor }
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
                        text: "⚡ Энергетический шар"
                        font.pixelSize: Theme.fontSizeHeading2
                        font.weight: Theme.fontWeightBold
                        color: Theme.adaptiveTextPrimary
                        wrapMode: Text.WordWrap
                    }

                    Item { height: Theme.paddingLarge }

                    // ОБНОВЛЕНО: Фаза упражнения (трехфазная методология Бронникова)
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 140
                        color: phaseColor
                        opacity: 0.15
                        radius: Theme.radiusMedium

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

            // Правая панель - Метрики и инструкции
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Theme.paddingMedium

                // Панель метрик
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 420  // Увеличено до 420 чтобы вместить ВСЕ метрики
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    GridLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        columns: 3
                        rowSpacing: 4  // Уменьшено для компактности
                        columnSpacing: Theme.paddingMedium

                        Text {
                            text: "📊 Метрики"
                            font.pixelSize: Theme.fontSizeHeading3
                            font.weight: Theme.fontWeightMedium
                            color: Theme.adaptiveTextPrimary
                            Layout.columnSpan: 3
                        }

                        // Alpha
                        Text {
                            text: "Alpha"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.alpha : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.alpha) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            color: Theme.alphaColor
                        }

                        // Beta
                        Text {
                            text: "Beta"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.beta : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.beta) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            color: Theme.betaColor
                        }

                        // Theta
                        Text {
                            text: "Theta"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.theta : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.theta) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            color: Theme.thetaColor
                        }

                        Item { height: Theme.paddingSmall; Layout.columnSpan: 3 }

                        // Concentration
                        Text {
                            text: "Концентрация"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.concentration : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.concentration) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            color: Theme.concentrationColor
                        }

                        // Relaxation
                        Text {
                            text: "Релаксация"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.relaxation : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.relaxation) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            color: Theme.relaxationColor
                        }

                        Item { height: Theme.paddingSmall; Layout.columnSpan: 3 }

                        // Heart Rate
                        Text {
                            text: "❤️ Пульс"
                            color: Theme.adaptiveTextSecondary
                            font.pixelSize: Theme.fontSizeBody
                        }
                        Item { Layout.fillWidth: true }
                        Text {
                            text: metricsModel ? metricsModel.heartRate + " BPM" : "0 BPM"
                            font.family: Theme.fontFamilyMono
                            font.pixelSize: Theme.fontSizeHeading3
                            color: Theme.heartRateColor
                        }

                        // Success Rate
                        Text {
                            text: "Success Rate"
                            color: Theme.adaptiveTextSecondary
                        }
                        ProgressBar {
                            Layout.fillWidth: true
                            from: 0; to: 100
                            value: metricsModel ? metricsModel.successRate : 0
                        }
                        Text {
                            text: metricsModel ? Math.round(metricsModel.successRate) + "%" : "0%"
                            font.family: Theme.fontFamilyMono
                            font.weight: Theme.fontWeightBold
                            color: Theme.successColor
                        }
                    }
                }

                // ОБНОВЛЕНО: Инструкции с цветом текущей фазы
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 140
                    color: Theme.surfaceColor
                    border.color: phaseColor
                    border.width: 2
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingSmall

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: Theme.paddingSmall

                            Text {
                                text: phaseIcon
                                font.pixelSize: 24
                            }

                            Text {
                                text: "Инструкция (" + phaseName + "):"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: Theme.adaptiveTextPrimary
                                Layout.fillWidth: true
                            }

                            // Индикатор паузы
                            Rectangle {
                                width: 80
                                height: 24
                                radius: 12
                                color: isPaused ? Theme.warningColor : "transparent"
                                visible: isPaused

                                Text {
                                    anchors.centerIn: parent
                                    text: "⏸️ ПАУЗА"
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.weight: Theme.fontWeightBold
                                    color: "white"
                                }
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            text: currentInstruction
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.adaptiveTextPrimary
                            wrapMode: Text.WordWrap
                        }

                        // Индикатор озвучивания
                        Row {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: Theme.paddingSmall
                            visible: audioController.isSpeaking

                            Rectangle {
                                width: 10
                                height: 10
                                radius: 5
                                color: Theme.accentColor
                                anchors.verticalCenter: parent.verticalCenter

                                SequentialAnimation on opacity {
                                    running: audioController.isSpeaking
                                    loops: Animation.Infinite
                                    NumberAnimation { from: 1.0; to: 0.3; duration: 500 }
                                    NumberAnimation { from: 0.3; to: 1.0; duration: 500 }
                                }
                            }

                            Text {
                                text: "🔊 Озвучивание..."
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.adaptiveTextSecondary
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }

                // НОВОЕ: Real-time график Alpha/Beta/Theta
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingSmall

                        Text {
                            text: "📈 Real-time график Alpha/Beta/Theta"
                            font.pixelSize: Theme.fontSizeHeading3
                            color: Theme.adaptiveTextPrimary
                        }

                        // ВРЕМЕННАЯ ЗАГЛУШКА: ChartView вызывает крэш в Qt 6.10.1
                        // TODO: Исправить после обновления Qt или найти workaround
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: Theme.backgroundColor
                            radius: Theme.radiusSmall
                            border.color: Theme.borderColor
                            border.width: 1

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingMedium
                                spacing: Theme.paddingSmall

                                // Простой текстовый график
                                Repeater {
                                    model: 3

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: Theme.paddingSmall

                                        Text {
                                            text: index === 0 ? "Alpha:" : index === 1 ? "Beta:" : "Theta:"
                                            font.pixelSize: Theme.fontSizeBody
                                            color: index === 0 ? Theme.alphaColor : index === 1 ? Theme.betaColor : Theme.thetaColor
                                            Layout.preferredWidth: 60
                                        }

                                        Rectangle {
                                            Layout.fillWidth: true
                                            height: 20
                                            radius: 10
                                            border.color: index === 0 ? Theme.alphaColor : index === 1 ? Theme.betaColor : Theme.thetaColor
                                            border.width: 2
                                            color: "transparent"

                                            Rectangle {
                                                height: parent.height
                                                radius: parent.radius
                                                color: index === 0 ? Theme.alphaColor : index === 1 ? Theme.betaColor : Theme.thetaColor
                                                opacity: 0.3
                                                width: {
                                                    if (!metricsModel) return 0
                                                    var value = index === 0 ? metricsModel.alpha : index === 1 ? metricsModel.beta : metricsModel.theta
                                                    return parent.width * value / 100
                                                }

                                                Behavior on width {
                                                    NumberAnimation { duration: 200 }
                                                }
                                            }
                                        }

                                        Text {
                                            text: {
                                                if (!metricsModel) return "0%"
                                                var value = index === 0 ? metricsModel.alpha : index === 1 ? metricsModel.beta : metricsModel.theta
                                                return Math.round(value) + "%"
                                            }
                                            font.pixelSize: Theme.fontSizeBody
                                            font.family: Theme.fontFamilyMono
                                            color: Theme.adaptiveTextPrimary
                                            Layout.preferredWidth: 50
                                        }
                                    }
                                }

                                Item { Layout.fillHeight: true }

                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: "⚠️ График временно недоступен\n(QtCharts compatibility issue)"
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.adaptiveTextSecondary
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
