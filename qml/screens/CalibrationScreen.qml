import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    id: calibrationScreen

    signal calibrationCompleted()
    signal calibrationCancelled()

    // Подключаемся к DeviceController
    property var deviceController: mainWindow ? mainWindow.deviceController : null

    // Локальные свойства
    property int remainingSeconds: 90
    property bool isCalibrating: deviceController ? deviceController.isCalibrating : false
    property int progress: deviceController ? deviceController.calibrationProgress : 0

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
            calibrationScreen.calibrationCompleted()
        }

        function onCalibrationFailed(error) {
            console.log("❌ Калибровка не удалась:", error)
            calibrationScreen.calibrationCancelled()
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
                Layout.preferredHeight: 150
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
                        lineHeight: 1.5
                    }
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
                visible: !calibrationScreen.isCalibrating && deviceController && deviceController.iaf > 0

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
                        if (calibrationScreen.isCalibrating && deviceController) {
                            deviceController.cancelCalibration()
                        }
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
                            console.log("Запуск калибровки...")
                            deviceController.startCalibration()
                        }
                    }
                }

                Button {
                    text: "✓ Продолжить"
                    visible: !calibrationScreen.isCalibrating && deviceController && deviceController.iaf > 0

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
