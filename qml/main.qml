import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "screens"
import "styles"

ApplicationWindow {
    id: mainWindow

    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: true
    title: "Bronnikov Method - Brain Training"

    // C++ модели и контроллеры - сохраняем reference для использования в handlers
    property var appMetricsModel: metricsModel
    property var appExerciseModel: exerciseModel
    property var appSessionModel: sessionModel
    property var appDeviceController: deviceController

    // Навигация - экспортируем stackView для доступа из других экранов
    property alias stackView: stackView

    // Экран калибровки - экспортируем для доступа из PreparationScreen
    property alias calibrationScreenComponent: calibrationScreen

    // Глобальная тема
    property var theme: Theme

    // Header с кнопкой меню
    header: Rectangle {
        width: parent.width
        height: 60
        color: Theme.primaryColor

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium
            spacing: Theme.paddingMedium

            // Кнопка гамбургер-меню
            Button {
                Layout.preferredWidth: 50
                Layout.preferredHeight: 50

                background: Rectangle {
                    color: parent.hovered ? Qt.darker(Theme.primaryColor, 1.1) : "transparent"
                    radius: Theme.radiusSmall
                }

                contentItem: Text {
                    text: "☰"
                    font.pixelSize: 28
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: drawer.open()
            }

            // Заголовок
            Text {
                Layout.fillWidth: true
                text: "Методика Бронникова"
                font.pixelSize: Theme.fontSizeHeading2
                font.weight: Theme.fontWeightBold
                color: "white"
                verticalAlignment: Text.AlignVCenter
            }

            // Статус устройства
            Button {
                Layout.preferredWidth: 120
                Layout.preferredHeight: 50

                background: Rectangle {
                    color: parent.hovered ? Qt.darker(Theme.primaryColor, 1.1) : "transparent"
                    radius: Theme.radiusSmall
                    border.color: deviceController && deviceController.isConnected ? Theme.successColor : Theme.errorColor
                    border.width: 1
                }

                contentItem: RowLayout {
                    spacing: 4

                    Text {
                        text: deviceController && deviceController.isConnected ? "📡" : "📡"
                        font.pixelSize: 20
                        color: deviceController && deviceController.isConnected ? Theme.successColor : "white"
                    }

                    Text {
                        text: deviceController && deviceController.isConnected ?
                              (deviceController.batteryLevel + "%") : "—"
                        font.pixelSize: Theme.fontSizeSmall
                        color: "white"
                        font.family: Theme.fontFamilyMono
                    }

                    Text {
                        text: deviceController && deviceController.isConnected ? "🔋" : ""
                        font.pixelSize: 16
                        color: deviceController && deviceController.batteryLevel < 20 ? Theme.errorColor : "white"
                        visible: deviceController && deviceController.isConnected
                    }
                }

                onClicked: {
                    deviceDialog.open()
                }

                ToolTip.visible: hovered
                ToolTip.text: deviceController ? deviceController.connectionStatus : "Не подключено"
            }
        }
    }

    // Боковое меню (Drawer)
    Drawer {
        id: drawer
        width: 280
        height: mainWindow.height

        Rectangle {
            anchors.fill: parent
            color: Theme.adaptiveBackground

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // Header меню
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 140
                    color: Theme.primaryColor

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingLarge
                        spacing: Theme.paddingSmall

                        Text {
                            text: "🧠"
                            font.pixelSize: 48
                            color: "white"
                        }

                        Text {
                            text: "Bronnikov Method"
                            font.pixelSize: Theme.fontSizeHeading3
                            font.weight: Theme.fontWeightBold
                            color: "white"
                        }

                        Text {
                            text: "Версия 0.10.0"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Qt.rgba(1, 1, 1, 0.7)
                        }
                    }
                }

                // Навигационное меню
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    model: ListModel {
                        ListElement { name: "Главная"; icon: "🏠"; screen: "home" }
                        ListElement { name: "Статистика"; icon: "📊"; screen: "statistics" }
                        ListElement { name: "Профиль"; icon: "👤"; screen: "profile" }
                        ListElement { name: "Настройки"; icon: "⚙️"; screen: "settings" }
                    }

                    delegate: ItemDelegate {
                        width: ListView.view.width
                        height: 60

                        background: Rectangle {
                            color: parent.hovered ? Theme.hoverColor : "transparent"
                        }

                        contentItem: RowLayout {
                            spacing: Theme.paddingMedium

                            Text {
                                text: model.icon
                                font.pixelSize: 24
                                Layout.preferredWidth: 40
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                text: model.name
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextPrimary
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "›"
                                font.pixelSize: 24
                                color: Theme.adaptiveTextSecondary
                            }
                        }

                        onClicked: {
                            drawer.close()

                            switch (model.screen) {
                                case "home":
                                    stackView.pop(null)  // Вернуться на главную
                                    break
                                case "statistics":
                                    stackView.pop(null)
                                    stackView.push(statisticsScreen)
                                    break
                                case "profile":
                                    stackView.pop(null)
                                    stackView.push(profileScreen)
                                    break
                                case "settings":
                                    stackView.pop(null)
                                    stackView.push(settingsScreen)
                                    break
                            }
                        }
                    }
                }

                // Divider
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: Theme.borderColor
                }

                // Footer (О приложении)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 80
                    color: "transparent"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Theme.paddingMedium
                        spacing: Theme.paddingSmall

                        Text {
                            text: "© 2024 Bronnikov Method"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                        }

                        Text {
                            text: "Qt " + Qt.application.version
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                        }
                    }
                }
            }
        }
    }

    // Модель для хранения найденных устройств
    ListModel {
        id: discoveredDevicesModel
    }

    // Подключение к сигналам deviceController для списка устройств
    Connections {
        target: deviceController

        function onDeviceDiscovered(deviceId, deviceName) {
            // Проверяем, нет ли уже этого устройства в списке
            for (var i = 0; i < discoveredDevicesModel.count; i++) {
                if (discoveredDevicesModel.get(i).deviceId === deviceId) {
                    return  // Уже есть
                }
            }
            discoveredDevicesModel.append({
                "deviceId": deviceId,
                "deviceName": deviceName
            })
        }

        function onSearchProgress(message) {
            // Очищаем список при начале нового поиска
            if (message === "Поиск устройств...") {
                discoveredDevicesModel.clear()
            }
        }
    }

    // Диалог подключения к устройству
    Dialog {
        id: deviceDialog
        title: "Управление устройством"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 500  // Увеличили высоту для списка устройств

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.paddingMedium

            // Статус подключения
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    RowLayout {
                        Layout.fillWidth: true

                        Text {
                            text: "📡 Статус:"
                            font.pixelSize: Theme.fontSizeHeading3
                            color: Theme.adaptiveTextPrimary
                        }

                        Item { Layout.fillWidth: true }

                        Rectangle {
                            width: 12
                            height: 12
                            radius: 6
                            color: deviceController && deviceController.isConnected ? Theme.successColor : Theme.errorColor
                        }

                        Text {
                            text: deviceController ? deviceController.connectionStatus : "Не подключено"
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.adaptiveTextPrimary
                        }
                    }

                    Text {
                        text: deviceController && deviceController.isConnected ?
                              ("Устройство: " + deviceController.deviceName) : "Устройство не подключено"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    RowLayout {
                        visible: deviceController && deviceController.isConnected
                        spacing: Theme.paddingMedium

                        Text {
                            text: "🔋 Батарея: " + (deviceController ? deviceController.batteryLevel : 0) + "%"
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.adaptiveTextSecondary
                        }

                        Text {
                            text: "📊 Качество: Ch1=" + (deviceController ? deviceController.signalQualityChannel1 : 0) +
                                  "% Ch2=" + (deviceController ? deviceController.signalQualityChannel2 : 0) + "%"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                        }
                    }
                }
            }

            // Список найденных устройств
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                color: Theme.surfaceColor
                radius: Theme.radiusMedium
                visible: discoveredDevicesModel.count > 0 && !(deviceController && deviceController.isConnected)

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    Text {
                        text: "📱 Найденные устройства (" + discoveredDevicesModel.count + "):"
                        font.pixelSize: Theme.fontSizeHeading3
                        color: Theme.adaptiveTextPrimary
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: discoveredDevicesModel
                        clip: true
                        spacing: 4

                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 36
                            color: mouseArea.containsMouse ? Theme.hoverColor : "transparent"
                            radius: Theme.radiusSmall

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    console.log("Подключение к устройству:", model.deviceId)
                                    if (deviceController) {
                                        deviceController.connectToDevice(model.deviceId)
                                    }
                                }
                            }

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingSmall
                                spacing: Theme.paddingMedium

                                Text {
                                    text: "📡"
                                    font.pixelSize: 16
                                }

                                Text {
                                    text: model.deviceName
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                    Layout.fillWidth: true
                                }

                                Text {
                                    text: "Подключить →"
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.primaryColor
                                    visible: mouseArea.containsMouse
                                }
                            }
                        }
                    }
                }
            }

            // Индикатор поиска/подключения
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: Theme.surfaceColor
                radius: Theme.radiusMedium
                visible: deviceController && (deviceController.connectionStatus === "Searching..." ||
                         deviceController.connectionStatus === "Connecting...")

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingMedium

                    BusyIndicator {
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                        running: true
                    }

                    Text {
                        Layout.fillWidth: true
                        text: deviceController ? deviceController.connectionStatus : ""
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.primaryColor
                        font.weight: Font.Medium
                    }
                }
            }

            // Кнопки управления
            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.paddingMedium

                Button {
                    Layout.fillWidth: true
                    text: "🔍 Поиск устройств"
                    enabled: deviceController && !deviceController.isConnected &&
                             deviceController.connectionStatus !== "Searching..." &&
                             deviceController.connectionStatus !== "Connecting..."

                    background: Rectangle {
                        color: parent.enabled ?
                               (parent.down ? Qt.darker(Theme.primaryColor, 1.1) :
                                parent.hovered ? Qt.lighter(Theme.primaryColor, 1.1) :
                                Theme.primaryColor) :
                               Theme.textDisabled
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
                        if (deviceController) {
                            deviceController.discoverDevices()
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: deviceController && deviceController.isConnected ? "❌ Отключить" : "✅ Подключить"
                    enabled: deviceController &&
                             deviceController.connectionStatus !== "Searching..." &&
                             deviceController.connectionStatus !== "Connecting..."

                    background: Rectangle {
                        color: parent.enabled ?
                               (parent.down ?
                                   Qt.darker(deviceController && deviceController.isConnected ? Theme.errorColor : Theme.successColor, 1.1) :
                                   parent.hovered ?
                                   Qt.lighter(deviceController && deviceController.isConnected ? Theme.errorColor : Theme.successColor, 1.1) :
                                   (deviceController && deviceController.isConnected ? Theme.errorColor : Theme.successColor)) :
                               Theme.textDisabled
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
                        if (deviceController) {
                            if (deviceController.isConnected) {
                                deviceController.disconnectFromDevice()
                            } else {
                                deviceController.connectToDevice("")
                            }
                        }
                    }
                }
            }

            // Калибровка
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                color: Theme.surfaceColor
                radius: Theme.radiusMedium
                visible: deviceController && deviceController.isSessionActive

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    Text {
                        text: "🎯 Калибровка NFB"
                        font.pixelSize: Theme.fontSizeHeading3
                        color: Theme.adaptiveTextPrimary
                    }

                    Text {
                        text: deviceController && deviceController.iaf > 0 ?
                              ("IAF: " + deviceController.iaf.toFixed(2) + " Hz, IAPF: " + deviceController.iapf.toFixed(2) + " Hz") :
                              "Калибровка не выполнена"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    Button {
                        text: deviceController && deviceController.isCalibrating ? "⏹️ Отменить калибровку" : "🎯 Запустить калибровку"
                        enabled: deviceController && deviceController.isSessionActive

                        background: Rectangle {
                            color: parent.enabled ?
                                   (parent.down ? Qt.darker(Theme.infoColor, 1.1) :
                                    parent.hovered ? Qt.lighter(Theme.infoColor, 1.1) :
                                    Theme.infoColor) :
                                   Theme.textDisabled
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
                            if (deviceController) {
                                if (deviceController.isCalibrating) {
                                    deviceController.cancelCalibration()
                                } else {
                                    deviceController.startCalibration()
                                }
                            }
                        }
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: deviceController ? deviceController.calibrationProgress : 0
                        visible: deviceController && deviceController.isCalibrating
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Кнопка закрытия
            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Закрыть"
                onClicked: deviceDialog.close()

                background: Rectangle {
                    color: parent.down ? Qt.darker(Theme.textDisabled, 1.1) :
                           parent.hovered ? Qt.lighter(Theme.textDisabled, 1.1) :
                           Theme.textDisabled
                    radius: Theme.radiusSmall
                }

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: Theme.textPrimary  // Темный текст на светлом фоне
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    // Навигация через StackView
    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: homeScreen

        // Анимация перехода между экранами
        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 300
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                property: "x"
                from: mainWindow.width * 0.1
                to: 0
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 300
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                property: "x"
                from: 0
                to: mainWindow.width * 0.1
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    }

    // Определение экранов как компоненты
    Component {
        id: homeScreen
        HomeScreen {
            onExerciseSelected: function(exerciseIndex) {
                // Получить имя упражнения из модели
                var exercise = exerciseModel.getExercise(exerciseIndex)
                var exerciseName = exercise ? exercise.name : ""
                console.log("[main] Выбрано упражнение:", exerciseName, "индекс:", exerciseIndex)
                stackView.push(preparationScreen, {
                    "exerciseIndex": exerciseIndex,
                    "exerciseName": exerciseName
                })
            }

            onShowStatistics: {
                stackView.push(statisticsScreen)
            }
        }
    }

    Component {
        id: preparationScreen
        PreparationScreen {
            onStartExercise: function(name) {
                console.log("[main] Запуск упражнения:", name)
                stackView.push(exerciseScreen, {
                    "exerciseName": name
                })
            }

            // onStartDemo обрабатывается через Connections в window

            onBack: {
                stackView.pop()
            }
        }
    }

    Component {
        id: exerciseScreen
        ExerciseScreen {
            onExerciseCompleted: {
                // НОВОЕ: Передать снимки метрик в ResultsScreen
                var resultsPage = resultsScreen.createObject(null, {
                    "baselineSnapshot": baselineSnapshot,
                    "activeSnapshot": activeSnapshot,
                    "postSnapshot": postSnapshot
                })
                stackView.push(resultsPage)
            }

            onBack: {
                stackView.pop()
            }
        }
    }

    Component {
        id: resultsScreen
        ResultsScreen {
            onReturnHome: {
                stackView.pop(null) // Вернуться к home
            }

            onRepeat: {
                stackView.pop()
                stackView.push(preparationScreen)
            }

            onShowStatistics: {
                stackView.push(statisticsScreen)
            }
        }
    }

    Component {
        id: statisticsScreen
        StatisticsScreen {
            onBack: {
                stackView.pop()
            }
        }
    }

    Component {
        id: profileScreen
        ProfileScreen {
            sessionModel: appSessionModel
            deviceController: appDeviceController
            onBack: {
                stackView.pop()
            }
        }
    }

    Component {
        id: settingsScreen
        SettingsScreen {
            onBack: {
                stackView.pop()
            }
        }
    }

    Component {
        id: calibrationScreen
        CalibrationScreen {
            onCalibrationCompleted: {
                console.log("[main] Калибровка завершена, возврат к подготовке")
                stackView.pop()
            }
            onCalibrationCancelled: {
                console.log("[main] Калибровка отменена, возврат к подготовке")
                stackView.pop()
            }
        }
    }

    // НОВОЕ: Connections для демо-режима на уровне window
    Connections {
        target: stackView.currentItem
        ignoreUnknownSignals: true

        function onStartDemo() {
            console.log("🎬 Signal startDemo получен в window")

            // Проверяем подключение устройства
            if (deviceController && deviceController.isConnected) {
                console.log("✓ Устройство подключено, запускаем реальную сессию")

                // Запускаем EEG сессию
                deviceController.startSession()

                // Запускаем мониторинг метрик
                if (mainWindow.appMetricsModel) {
                    mainWindow.appMetricsModel.startMonitoring()
                }

                // Переходим к экрану упражнения
                stackView.push(exerciseScreen)
            } else {
                console.log("⚠️ Устройство не подключено, запускаем демо-режим")

                if (mainWindow.appMetricsModel) {
                    mainWindow.appMetricsModel.startDemoMode()
                    stackView.push(exerciseScreen)
                } else {
                    console.log("✗ appMetricsModel не найден!")
                }
            }
        }
    }
}
