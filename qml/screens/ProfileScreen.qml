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
                    text: "👤 Профиль пользователя"
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

                    // Информация о пользователе
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
                                text: "📋 Основная информация"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Имя:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: "Пользователь"  // TODO: получать из SessionModel
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#1a1a1a"
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Дата регистрации:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: "2024-01-15"  // TODO: получать из SessionModel
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#1a1a1a"
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Всего сессий:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#666666"
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: sessionModel ? sessionModel.totalSessions : "0"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: "#1a1a1a"
                                }
                            }
                        }
                    }

                    // Baseline метрики
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 280
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingSmall

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "📊 Baseline метрики"
                                    font.pixelSize: Theme.fontSizeHeading3
                                    font.weight: Theme.fontWeightMedium
                                    color: "#1a1a1a"
                                }

                                Item { Layout.fillWidth: true }

                                Text {
                                    text: "Обновлено: 2024-02-04"  // TODO: дата последней калибровки
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: "#666666"
                                }
                            }

                            GridLayout {
                                Layout.fillWidth: true
                                columns: 3
                                rowSpacing: Theme.paddingSmall
                                columnSpacing: Theme.paddingMedium

                                // Alpha
                                Text {
                                    text: "Alpha:"
                                    color: "#666666"
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: 45  // TODO: получать из SessionModel baseline
                                }
                                Text {
                                    text: "45%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.alphaColor
                                }

                                // Beta
                                Text {
                                    text: "Beta:"
                                    color: "#666666"
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: 35
                                }
                                Text {
                                    text: "35%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.betaColor
                                }

                                // Theta
                                Text {
                                    text: "Theta:"
                                    color: "#666666"
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: 20
                                }
                                Text {
                                    text: "20%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.thetaColor
                                }

                                // IAF
                                Text {
                                    text: "IAF:"
                                    color: "#666666"
                                }
                                Item { Layout.fillWidth: true }
                                Text {
                                    text: "10.2 Hz"  // TODO: из SessionModel
                                    font.family: Theme.fontFamilyMono
                                    color: "#1a1a1a"
                                }

                                // Heart Rate
                                Text {
                                    text: "Пульс (покой):"
                                    color: "#666666"
                                }
                                Item { Layout.fillWidth: true }
                                Text {
                                    text: "72 BPM"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.heartRateColor
                                }
                            }

                            Item { Layout.fillHeight: true }

                            Button {
                                Layout.alignment: Qt.AlignHCenter
                                text: "🔄 Повторить калибровку"

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
                                    // TODO: запустить калибровку
                                    console.log("Запуск калибровки...")
                                }
                            }
                        }
                    }

                    // Последние сессии
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingSmall

                            Text {
                                text: "📅 Последние сессии"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: "#1a1a1a"
                            }

                            ListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 8
                                clip: true

                                model: 5  // TODO: получать из SessionModel

                                delegate: Rectangle {
                                    width: ListView.view.width
                                    height: 30
                                    color: "transparent"

                                    RowLayout {
                                        anchors.fill: parent
                                        spacing: Theme.paddingMedium

                                        Text {
                                            text: "2024-02-0" + (5 - index)
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: "#666666"
                                            Layout.preferredWidth: 100
                                        }

                                        Text {
                                            text: "Энергетический шар"
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: "#1a1a1a"
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: (70 + index * 3) + "%"
                                            font.pixelSize: Theme.fontSizeSmall
                                            font.family: Theme.fontFamilyMono
                                            color: Theme.successColor
                                            Layout.preferredWidth: 50
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
