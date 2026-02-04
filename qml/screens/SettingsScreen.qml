import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
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
                                    currentIndex: 0

                                    contentItem: Text {
                                        leftPadding: 12
                                        rightPadding: 40
                                        text: parent.displayText
                                        font: parent.font
                                        color: "#1a1a1a"
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onCurrentIndexChanged: {
                                        console.log("Тема изменена на:", currentIndex)
                                        // TODO: применить тему
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
                                    currentIndex: 0

                                    contentItem: Text {
                                        leftPadding: 12
                                        rightPadding: 40
                                        text: parent.displayText
                                        font: parent.font
                                        color: "#1a1a1a"
                                        verticalAlignment: Text.AlignVCenter
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
                                    checked: true
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
                                    checked: true
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
                                    checked: false

                                    onCheckedChanged: {
                                        console.log("Демо-режим:", checked ? "включен" : "выключен")
                                        // TODO: переключить режим в DeviceController
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
                                    text: "NeiryBand #1234"  // TODO: из DeviceController
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
                                    value: 10
                                    stepSize: 5

                                    textFromValue: function(value) {
                                        return value + " мин"
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
                                    checked: true
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
                                    checked: false
                                }
                            }
                        }
                    }

                    // Настройки данных
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
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
                                    checked: true
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
                                        console.log("Экспорт данных...")
                                        // TODO: экспорт в CSV
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
                                        console.log("Очистка истории...")
                                        // TODO: показать подтверждение и очистить БД
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
}
