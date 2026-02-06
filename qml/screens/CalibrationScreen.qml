import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    id: calibrationScreen

    signal calibrationCompleted()
    signal calibrationCancelled()

    // ПРИМЕЧАНИЕ: deviceController доступен глобально через QML context (см. main_gui.cpp:132)
    // Не нужно получать его через mainWindow

    // Локальные свойства
    property int remainingSeconds: 90
    property bool isCalibrating: deviceController ? deviceController.isCalibrating : false
    property int progress: deviceController ? deviceController.calibrationProgress : 0
    property bool calibrationSuccessful: false
    property string errorMessage: ""

    // Таймер для запуска калибровки после старта сессии
    Timer {
        id: delayedCalibrationTimer
        interval: 2000  // 2 секунды на запуск сессии
        repeat: false
        onTriggered: {
            if (deviceController && deviceController.isSessionActive) {
                console.log("Сессия активна, запуск калибровки...")
                deviceController.startCalibration()
            } else {
                console.log("Сессия всё ещё не активна")
                calibrationScreen.errorMessage = "Не удалось запустить сессию"
            }
        }
    }

    Connections {
        target: deviceController

        function onIsCalibratingChanged() {
            calibrationScreen.isCalibrating = deviceController.isCalibrating
        }

        function onCalibrationProgressChanged() {
            calibrationScreen.progress = deviceController.calibrationProgress
            calibrationScreen.remainingSeconds = Math.max(0, 90 - Math.floor(90 * calibrationScreen.progress / 100))
        }

        function onCalibrationCompleted(iaf, iapf) {
            console.log("✓ Калибровка завершена! IAF:", iaf, "IAPF:", iapf)
            calibrationScreen.calibrationSuccessful = true
            calibrationScreen.errorMessage = ""
        }

        function onCalibrationFailed(error) {
            console.log("❌ Калибровка не удалась:", error)
            // НЕ выходим автоматически - показываем ошибку
            calibrationScreen.errorMessage = error
        }
    }

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.6
            spacing: Theme.paddingLarge

            // Заголовок
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "🎯 Калибровка NFB"
                font.pixelSize: Theme.fontSizeDisplay
                font.weight: Theme.fontWeightBold
                color: Theme.adaptiveTextPrimary
            }

            // Инструкции
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 200  // УВЕЛИЧЕНО с 150 до 200 для 4 пунктов
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingMedium

                    Text {
                        Layout.fillWidth: true
                        text: "Инструкция:"
                        font.pixelSize: Theme.fontSizeHeading2
                        font.weight: Theme.fontWeightBold
                        color: Theme.adaptiveTextPrimary
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "1. Сядьте удобно и расслабьтесь\n2. Закройте глаза\n3. Дышите спокойно и ровно\n4. Не двигайтесь в течение 90 секунд"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                        wrapMode: Text.WordWrap
                        lineHeight: 1.4  // УМЕНЬШЕНО с 1.5 до 1.4 для компактности
                    }
                }
            }

            // Сообщение об ошибке
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                color: Theme.errorColor
                radius: Theme.radiusMedium
                visible: calibrationScreen.errorMessage !== ""

                Text {
                    anchors.centerIn: parent
                    text: "❌ " + calibrationScreen.errorMessage
                    font.pixelSize: Theme.fontSizeBody
                    color: "white"
                    wrapMode: Text.WordWrap
                }
            }

            // Прогресс
            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.paddingMedium
                visible: calibrationScreen.isCalibrating

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Осталось: " + calibrationScreen.remainingSeconds + " сек"
                    font.pixelSize: Theme.fontSizeHeading1
                    color: Theme.adaptiveTextPrimary
                }

                ProgressBar {
                    Layout.fillWidth: true
                    from: 0
                    to: 100
                    value: calibrationScreen.progress

                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 12
                        color: Theme.surfaceColor
                        radius: 6
                    }

                    contentItem: Item {
                        implicitWidth: 200
                        implicitHeight: 12

                        Rectangle {
                            width: calibrationScreen.progress * parent.width / 100
                            height: parent.height
                            radius: 6
                            color: Theme.successColor
                        }
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: calibrationScreen.progress + "%"
                    font.pixelSize: Theme.fontSizeHeading3
                    color: Theme.adaptiveTextSecondary
                }
            }

            // Результат (после завершения)
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                color: Theme.surfaceColor
                radius: Theme.radiusMedium
                visible: !calibrationScreen.isCalibrating && calibrationScreen.calibrationSuccessful

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingLarge
                    spacing: Theme.paddingSmall

                    Text {
                        text: "✓ Калибровка завершена успешно!"
                        font.pixelSize: Theme.fontSizeHeading2
                        color: Theme.successColor
                    }

                    Text {
                        text: "Ваша индивидуальная альфа-частота (IAF): " +
                              (deviceController ? deviceController.iaf.toFixed(2) : "0") + " Hz"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Кнопки
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: Theme.paddingMedium

                Button {
                    text: calibrationScreen.isCalibrating ? "⏹️ Отменить" : "← Назад"
                    visible: true

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
                        console.log("🔙 Кнопка Назад нажата!")
                        if (calibrationScreen.isCalibrating && deviceController) {
                            console.log("  Отмена калибровки...")
                            deviceController.cancelCalibration()
                        }
                        console.log("  Возврат назад...")
                        calibrationScreen.calibrationCancelled()
                    }
                }

                Button {
                    text: "▶️ Начать калибровку"
                    visible: !calibrationScreen.isCalibrating

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

                    onClicked: {
                        if (deviceController) {
                            calibrationScreen.errorMessage = ""  // Очищаем предыдущую ошибку

                            // Проверяем, активна ли сессия
                            if (!deviceController.isSessionActive) {
                                console.log("Сессия не активна, запускаем...")
                                deviceController.startSession()

                                // Даем время на запуск сессии и запускаем калибровку с задержкой
                                delayedCalibrationTimer.start()
                            } else {
                                console.log("Запуск калибровки...")
                                deviceController.startCalibration()
                            }
                        }
                    }
                }

                Button {
                    text: "✓ Продолжить"
                    visible: !calibrationScreen.isCalibrating && calibrationScreen.calibrationSuccessful

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

                    onClicked: {
                        calibrationScreen.calibrationCompleted()
                    }
                }
            }
        }
    }
}
