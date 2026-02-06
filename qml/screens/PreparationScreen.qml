import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    id: preparationScreen

    property int exerciseIndex: 0
    property string exerciseName: ""
    property string statusMessage: deviceController && deviceController.isConnected ? "Устройство подключено" : "Ожидание подключения..."
    property bool isConnecting: false

    signal startExercise(string name)
    signal startDemo()  // НОВОЕ: запуск в демо-режиме
    signal back()

    // Обновляем статус при загрузке экрана
    Component.onCompleted: {
        if (deviceController && deviceController.isConnected) {
            preparationScreen.statusMessage = "Устройство подключено"
            preparationScreen.isConnecting = false
        }
    }

    // НОВОЕ: Подключение к сигналам DeviceController для прогресса
    // ПРИМЕЧАНИЕ: deviceController доступен глобально через QML context (см. main_gui.cpp:132)
    Connections {
        target: deviceController

        function onConnectionProgress(message) {
            preparationScreen.statusMessage = message
            preparationScreen.isConnecting = true
        }

        function onSearchProgress(message) {
            preparationScreen.statusMessage = message
            preparationScreen.isConnecting = true
        }

        function onIsConnectedChanged() {
            if (deviceController.isConnected) {
                preparationScreen.statusMessage = "Подключено"
                preparationScreen.isConnecting = false
            }
        }

        function onConnectionFailed(error) {
            preparationScreen.statusMessage = "Ошибка: " + error
            preparationScreen.isConnecting = false
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingLarge

            // Header с кнопкой назад
            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "← Назад"

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

                    onClicked: back()
                }

                Text {
                    text: preparationScreen.exerciseName || "Подготовка к упражнению"
                    font.pixelSize: Theme.fontSizeHeading1
                    font.weight: Theme.fontWeightBold
                    color: Theme.adaptiveTextPrimary
                }

                Item { Layout.fillWidth: true }
            }

            // Прогресс подготовки
            Text {
                text: "Шаг 1 из 3: Подключение к устройству"
                font.pixelSize: Theme.fontSizeHeading2
                color: Theme.adaptiveTextPrimary
            }

            ProgressBar {
                Layout.fillWidth: true
                from: 0
                to: 100
                value: 33  // 1 из 3 шагов
            }

            // Статус подключения
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingMedium

                    Text {
                        text: "🔌 Статус подключения"
                        font.pixelSize: Theme.fontSizeHeading2
                        color: Theme.adaptiveTextPrimary
                    }

                    // НОВОЕ: Реальный статус с BusyIndicator
                    RowLayout {
                        spacing: Theme.paddingMedium

                        BusyIndicator {
                            Layout.preferredWidth: 32
                            Layout.preferredHeight: 32
                            running: preparationScreen.isConnecting
                            visible: preparationScreen.isConnecting
                        }

                        Text {
                            text: preparationScreen.statusMessage
                            font.pixelSize: Theme.fontSizeBody
                            color: preparationScreen.isConnecting ? Theme.warningColor :
                                   (deviceController && deviceController.isConnected ? Theme.successColor : Theme.adaptiveTextPrimary)
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                    }

                    // Информация о подключенном устройстве
                    Column {
                        spacing: Theme.paddingSmall
                        visible: deviceController && deviceController.isConnected

                        Text {
                            text: "✓ Устройство: " + (deviceController ? deviceController.deviceName : "")
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.successColor
                        }
                    }

                    Item { height: Theme.paddingLarge }

                    Text {
                        text: "📊 Качество сигнала:"
                        font.pixelSize: Theme.fontSizeHeading3
                        color: Theme.adaptiveTextPrimary
                    }

                    // Качество каналов
                    Column {
                        spacing: Theme.paddingSmall

                        Row {
                            spacing: Theme.paddingMedium
                            Text {
                                text: "Канал 1:"
                                width: 80
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }
                            ProgressBar {
                                width: 200
                                from: 0
                                to: 100
                                value: deviceController ? deviceController.signalQualityChannel1 : 0
                            }
                            Text {
                                property int quality: deviceController ? deviceController.signalQualityChannel1 : 0
                                text: quality + "% (" + (quality > 80 ? "отлично" : quality > 60 ? "хорошо" : quality > 40 ? "средне" : "плохо") + ")"
                                font.pixelSize: Theme.fontSizeBody
                                color: quality > 80 ? Theme.successColor : quality > 60 ? Theme.warningColor : Theme.errorColor
                            }
                        }

                        Row {
                            spacing: Theme.paddingMedium
                            Text {
                                text: "Канал 2:"
                                width: 80
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextSecondary
                            }
                            ProgressBar {
                                width: 200
                                from: 0
                                to: 100
                                value: deviceController ? deviceController.signalQualityChannel2 : 0
                            }
                            Text {
                                property int quality: deviceController ? deviceController.signalQualityChannel2 : 0
                                text: quality + "% (" + (quality > 80 ? "отлично" : quality > 60 ? "хорошо" : quality > 40 ? "средне" : "плохо") + ")"
                                font.pixelSize: Theme.fontSizeBody
                                color: quality > 80 ? Theme.successColor : quality > 60 ? Theme.warningColor : Theme.errorColor
                            }
                        }
                    }

                    Item { height: Theme.paddingLarge }

                    Text {
                        property int battery: deviceController ? deviceController.batteryLevel : 0
                        text: "🔋 Батарея: " + battery + "%"
                        font.pixelSize: Theme.fontSizeBody
                        color: battery < 20 ? Theme.errorColor : battery < 50 ? Theme.warningColor : Theme.successColor
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // Настройки упражнения
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    Text {
                        text: "Настройки упражнения"
                        font.pixelSize: Theme.fontSizeHeading3
                        font.weight: Theme.fontWeightBold
                        color: Theme.adaptiveTextPrimary
                    }

                    // Выбор длительности
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Theme.paddingLarge

                        Text {
                            text: "Длительность:"
                            font.pixelSize: Theme.fontSizeBody
                            font.weight: Theme.fontWeightMedium
                            color: Theme.adaptiveTextPrimary
                        }

                        // Группа для исключительного выбора
                        ButtonGroup { id: durationGroup }

                        // Рекомендуемая
                        Row {
                            spacing: 6

                            RadioButton {
                                id: autoRadio
                                checked: true
                                ButtonGroup.group: durationGroup
                                onCheckedChanged: {
                                    if (checked && exerciseController) {
                                        var level = sessionModel ? sessionModel.practiceLevel : 0;
                                        exerciseController.setRecommendedDuration(level, false);
                                    }
                                }
                            }
                            Text {
                                text: {
                                    if (sessionModel) {
                                        var level = sessionModel.practiceLevel || 0;
                                        var ranges = ["1-2 мин (новичок)", "2-5 мин (практик)", "5-10 мин (эксперт)"];
                                        return "Рекомендуемая: " + ranges[level];
                                    }
                                    return "Рекомендуемая: 1-2 мин";
                                }
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextPrimary
                                anchors.verticalCenter: parent.verticalCenter
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: autoRadio.checked = true
                                }
                            }
                        }

                        // Вручную
                        Row {
                            spacing: 6

                            RadioButton {
                                id: manualRadio
                                ButtonGroup.group: durationGroup
                                onCheckedChanged: {
                                    if (checked && exerciseController) {
                                        exerciseController.setDuration(durationSpinBox.value);
                                    }
                                }
                            }
                            Text {
                                text: "Вручную:"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.adaptiveTextPrimary
                                anchors.verticalCenter: parent.verticalCenter
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: manualRadio.checked = true
                                }
                            }
                            SpinBox {
                                id: durationSpinBox
                                from: 1
                                to: 20
                                value: 5
                                enabled: manualRadio.checked
                                width: 100

                                textFromValue: function(value) { return value + " мин"; }
                                valueFromText: function(text) { return parseInt(text); }

                                onValueChanged: {
                                    if (manualRadio.checked && exerciseController) {
                                        exerciseController.setDuration(value);
                                    }
                                }
                            }
                        }
                    }

                    // Итоговая длительность
                    Rectangle {
                        Layout.fillWidth: true
                        height: 40
                        color: Qt.rgba(Theme.primaryColor.r, Theme.primaryColor.g, Theme.primaryColor.b, 0.1)
                        radius: Theme.radiusSmall
                        border.color: Theme.primaryColor
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: "Длительность упражнения: " + (exerciseController ? exerciseController.duration : 5) + " минут"
                            font.pixelSize: Theme.fontSizeBody
                            color: Theme.primaryColor
                            font.weight: Theme.fontWeightBold
                        }
                    }
                }
            }

            // Кнопки управления
            RowLayout {
                Layout.fillWidth: true

                // Кнопка демо-режима
                Button {
                    text: "🎭 Демо-режим"
                    font.pixelSize: Theme.fontSizeBody

                    background: Rectangle {
                        color: parent.down ? Qt.darker(Theme.warningColor, 1.1) :
                               parent.hovered ? Qt.lighter(Theme.warningColor, 1.05) :
                               Theme.warningColor
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
                        console.log("🎭 Запуск демо-режима")
                        startDemo()
                    }
                }

                // Кнопка калибровки
                Button {
                    text: "🎯 Калибровка"
                    font.pixelSize: Theme.fontSizeBody
                    visible: deviceController && deviceController.isConnected
                    enabled: deviceController && deviceController.isConnected  // Активна когда подключено

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
                        console.log("🎯 Переход к калибровке")
                        // Сначала запускаем сессию если еще не запущена
                        if (deviceController && !deviceController.isSessionActive) {
                            deviceController.startSession()
                        }
                        // Переходим к экрану калибровки через Component
                        mainWindow.stackView.push(mainWindow.calibrationScreenComponent)
                    }
                }

                Item { Layout.fillWidth: true }

                // Кнопка продолжить
                Button {
                    text: "Продолжить →"
                    font.pixelSize: Theme.fontSizeBody
                    enabled: deviceController && deviceController.isConnected  // Активна когда устройство подключено

                    background: Rectangle {
                        color: parent.enabled ?
                               (parent.down ? Qt.darker(Theme.successColor, 1.1) :
                                parent.hovered ? Qt.lighter(Theme.successColor, 1.1) :
                                Theme.successColor) :
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
                        console.log("[PreparationScreen] Запуск упражнения:", preparationScreen.exerciseName)
                        startExercise(preparationScreen.exerciseName)
                    }
                }
            }
        }
    }
}
