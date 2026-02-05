import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import "../styles"

Item {
    signal back()

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
                    onClicked: back()

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
                }

                Text {
                    text: "⚙️ Настройки"
                    font.pixelSize: Theme.fontSizeHeading1
                    font.weight: Theme.fontWeightBold
                    color: Theme.adaptiveTextPrimary
                }

                Item { Layout.fillWidth: true }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: Theme.paddingMedium

                    // Настройки интерфейса
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingMedium

                            Text {
                                text: "🎨 Интерфейс"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Тема оформления:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                ComboBox {
                                    Layout.preferredWidth: 200
                                    model: ["Светлая", "Темная", "Авто"]
                                    currentIndex: settingsController ? settingsController.themeIndex : 0

                                    contentItem: Text {
                                        leftPadding: 12
                                        rightPadding: 40
                                        text: parent.displayText
                                        font: parent.font
                                        color: "#1a1a1a"
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onCurrentIndexChanged: {
                                        if (settingsController) {
                                            settingsController.themeIndex = currentIndex
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Язык:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                ComboBox {
                                    Layout.preferredWidth: 200
                                    model: ["Русский", "English"]
                                    currentIndex: settingsController ? settingsController.languageIndex : 0

                                    contentItem: Text {
                                        leftPadding: 12
                                        rightPadding: 40
                                        text: parent.displayText
                                        font: parent.font
                                        color: "#1a1a1a"
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onCurrentIndexChanged: {
                                        if (settingsController) {
                                            settingsController.languageIndex = currentIndex
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Показывать подсказки:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                Switch {
                                    checked: settingsController ? settingsController.showHints : true
                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.showHints = checked
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Настройки устройства
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 180
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingMedium

                            Text {
                                text: "📡 Устройство"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Автоподключение:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                Switch {
                                    checked: settingsController ? settingsController.autoConnect : true
                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.autoConnect = checked
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Использовать демо-режим:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                Switch {
                                    id: demoModeSwitch
                                    checked: settingsController ? settingsController.demoMode : false

                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.demoMode = checked
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Последнее устройство:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 200
                                }

                                Text {
                                    text: settingsController && settingsController.lastDevice ?
                                          settingsController.lastDevice :
                                          "Не подключено"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#1a1a1a"
                                }
                            }
                        }
                    }

                    // Настройки упражнений
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 180
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingMedium

                            Text {
                                text: "🧘 Упражнения"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Длительность по умолчанию:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 220
                                }

                                SpinBox {
                                    from: 5
                                    to: 60
                                    value: settingsController ? settingsController.defaultDuration : 10
                                    stepSize: 5

                                    textFromValue: function(value) {
                                        return value + " мин"
                                    }

                                    onValueChanged: {
                                        if (settingsController) {
                                            settingsController.defaultDuration = value
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Звуковые сигналы:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 220
                                }

                                Switch {
                                    checked: settingsController ? settingsController.soundSignals : true
                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.soundSignals = checked
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Голосовые инструкции:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 220
                                }

                                Switch {
                                    checked: settingsController ? settingsController.voiceInstructions : false
                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.voiceInstructions = checked
                                        }
                                    }
                                }
                            }

                            // Громкость звуков
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: Theme.paddingSmall

                                Text {
                                    text: "Громкость звуков:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: Theme.paddingMedium

                                    Slider {
                                        Layout.fillWidth: true
                                        from: 0.0
                                        to: 1.0
                                        value: audioController ? audioController.volume : 0.7
                                        enabled: settingsController ? settingsController.soundSignals : true

                                        onValueChanged: {
                                            if (audioController) {
                                                audioController.volume = value
                                            }
                                        }
                                    }

                                    Text {
                                        text: Math.round((audioController ? audioController.volume : 0.7) * 100) + "%"
                                        font.pixelSize: Theme.fontSizeSmall
                                        color: "#999999"
                                        Layout.preferredWidth: 40
                                    }
                                }
                            }

                            // Скорость речи
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: Theme.paddingSmall

                                Text {
                                    text: "Скорость речи:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: Theme.paddingMedium

                                    Slider {
                                        Layout.fillWidth: true
                                        from: 0.5
                                        to: 2.0
                                        value: audioController ? audioController.rate : 1.0
                                        enabled: settingsController ? settingsController.voiceInstructions : false

                                        onValueChanged: {
                                            if (audioController) {
                                                audioController.rate = value
                                            }
                                        }
                                    }

                                    Text {
                                        text: (audioController ? audioController.rate : 1.0).toFixed(1) + "x"
                                        font.pixelSize: Theme.fontSizeSmall
                                        color: "#999999"
                                        Layout.preferredWidth: 40
                                    }
                                }
                            }
                        }
                    }

                    // Настройки данных
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingMedium

                            Text {
                                text: "💾 Данные"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Автосохранение CSV:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 220
                                }

                                Switch {
                                    checked: settingsController ? settingsController.autosaveCSV : true
                                    onCheckedChanged: {
                                        if (settingsController) {
                                            settingsController.autosaveCSV = checked
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Research Mode (raw EEG/PPG/MEMS):"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 220
                                }

                                Switch {
                                    id: researchModeSwitch
                                    checked: settingsController ? settingsController.researchMode : false
                                    onCheckedChanged: {
                                        if (checked && settingsController && settingsController.researchMode !== checked) {
                                            // Показываем предупреждение при включении
                                            researchModeWarningDialog.open()
                                        } else if (settingsController) {
                                            settingsController.researchMode = checked
                                        }
                                    }
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: Theme.paddingMedium

                                Button {
                                    text: "Экспорт данных"

                                    background: Rectangle {
                                        color: parent.down ? Qt.darker(Theme.infoColor, 1.1) :
                                               parent.hovered ? Qt.lighter(Theme.infoColor, 1.1) :
                                               Theme.infoColor
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
                                        if (settingsController) {
                                            settingsController.exportAllData("default")
                                        }
                                    }
                                }

                                Button {
                                    text: "Очистить историю"

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
                                        confirmDialog.open()
                                    }
                                }
                            }
                        }
                    }

                    // О приложении
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingSmall

                            Text {
                                text: "ℹ️ О приложении"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            Text {
                                text: "Bronnikov Exercise App"
                                font.pixelSize: Theme.fontSizeBody
                                color: "#1a1a1a"
                            }

                            Text {
                                text: "Версия: 0.10.0"
                                font.pixelSize: Theme.fontSizeSmall
                                color: "#666666"
                            }

                            Text {
                                text: "Qt версия: 6.10.1"
                                font.pixelSize: Theme.fontSizeSmall
                                color: "#666666"
                            }

                            Text {
                                text: "© 2024 Bronnikov Method"
                                font.pixelSize: Theme.fontSizeSmall
                                color: "#666666"
                            }
                        }
                    }
                }
            }
        }
    }

    // Диалог предупреждения о Research Mode
    Dialog {
        id: researchModeWarningDialog
        title: "⚠️ Research Mode"
        modal: true
        anchors.centerIn: parent
        width: 500
        standardButtons: Dialog.Ok | Dialog.Cancel

        contentItem: ColumnLayout {
            spacing: 10

            Text {
                text: "Research Mode включает экспорт RAW EEG/PPG/MEMS данных."
                font.pixelSize: Theme.fontSizeBody
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                text: "⚠️ ВНИМАНИЕ:"
                font.pixelSize: Theme.fontSizeBody
                font.weight: Theme.fontWeightBold
                color: Theme.errorColor
            }

            Text {
                text: "• Будут создаваться 3 дополнительных CSV файла на сессию\n" +
                      "• Размер файлов может достигать 10-50 МБ каждый\n" +
                      "• Используйте только для исследовательских целей\n" +
                      "• Данные сохраняются в data/Users/[user]/research/"
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                color: "#666666"
            }

            Text {
                text: "Вы уверены, что хотите включить Research Mode?"
                font.pixelSize: Theme.fontSizeBody
                font.weight: Theme.fontWeightMedium
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }

        onAccepted: {
            if (settingsController) {
                settingsController.researchMode = true
            }
        }

        onRejected: {
            researchModeSwitch.checked = false
        }
    }

    // Диалог подтверждения очистки истории
    Dialog {
        id: confirmDialog
        title: "Подтверждение"
        modal: true
        anchors.centerIn: parent
        width: 400
        standardButtons: Dialog.Yes | Dialog.No

        contentItem: Text {
            text: "Вы уверены, что хотите очистить историю сессий?\n\nБудут удалены сессии старше 7 дней."
            wrapMode: Text.WordWrap
        }

        onAccepted: {
            if (settingsController) {
                settingsController.clearHistory("default")
            }
        }
    }
}
