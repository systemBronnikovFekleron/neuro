import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    id: preparationScreen

    property int exerciseIndex: 0
    property string statusMessage: "Ожидание подключения..."
    property bool isConnecting: false

    signal startExercise()
    signal startDemo()  // НОВОЕ: запуск в демо-режиме
    signal back()

    // НОВОЕ: Подключение к сигналам DeviceController для прогресса
    Connections {
        target: mainWindow.deviceController

        function onConnectionProgress(message) {
            preparationScreen.statusMessage = message
            preparationScreen.isConnecting = true
        }

        function onSearchProgress(message) {
            preparationScreen.statusMessage = message
            preparationScreen.isConnecting = true
        }

        function onIsConnectedChanged() {
            if (mainWindow.deviceController.isConnected) {
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
                    text: "Подготовка к упражнению"
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
                                   (mainWindow.deviceController && mainWindow.deviceController.isConnected ? Theme.successColor : Theme.adaptiveTextPrimary)
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                    }

                    // Информация о подключенном устройстве
                    Column {
                        spacing: Theme.paddingSmall
                        visible: mainWindow.deviceController && mainWindow.deviceController.isConnected

                        Text {
                            text: "✓ Устройство: " + (mainWindow.deviceController ? mainWindow.deviceController.deviceName : "")
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
                                value: 80
                            }
                            Text {
                                text: "80% (хорошо)"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.successColor
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
                                value: 70
                            }
                            Text {
                                text: "70% (хорошо)"
                                font.pixelSize: Theme.fontSizeBody
                                color: Theme.successColor
                            }
                        }
                    }

                    Item { height: Theme.paddingLarge }

                    Text {
                        text: "🔋 Батарея: 73%"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                    }

                    Item { Layout.fillHeight: true }
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
                    visible: mainWindow.deviceController && mainWindow.deviceController.isConnected
                    enabled: !preparationScreen.isConnecting

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
                        if (mainWindow.deviceController && !mainWindow.deviceController.isSessionActive) {
                            mainWindow.deviceController.startSession()
                        }
                        // Переходим к экрану калибровки
                        mainWindow.stackView.push("qrc:/screens/CalibrationScreen.qml")
                    }
                }

                Item { Layout.fillWidth: true }

                // Кнопка продолжить
                Button {
                    text: "Продолжить →"
                    font.pixelSize: Theme.fontSizeBody
                    enabled: false  // Включается когда устройство подключено

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
                        // TODO: Перейти к калибровке или инструкциям
                        startExercise()
                    }
                }
            }
        }
    }
}
