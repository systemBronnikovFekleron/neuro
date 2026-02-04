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

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "← Назад"
                    onClicked: back()
                }

                Text {
                    text: "Статистика и прогресс"
                    font.pixelSize: Theme.fontSizeHeading1
                    font.weight: Theme.fontWeightBold
                    color: Theme.adaptiveTextPrimary
                }

                Item { Layout.fillWidth: true }
            }

            TabBar {
                id: tabBar
                Layout.fillWidth: true

                TabButton {
                    text: "Общее"
                }
                TabButton {
                    text: "По упражнениям"
                }
                TabButton {
                    text: "По ступеням"
                }
                TabButton {
                    text: "Календарь"
                }
            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: tabBar.currentIndex

                // Общее
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    Text {
                        anchors.centerIn: parent
                        text: "📊 Общая статистика\n\nВсего сессий: 47\nВсего часов: 8.5\nСредний успех: 72%"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                // По упражнениям
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    Text {
                        anchors.centerIn: parent
                        text: "Статистика по упражнениям"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                    }
                }

                // По ступеням
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    Text {
                        anchors.centerIn: parent
                        text: "📈 Прогресс по ступеням\n\nПодготовительная: 80%\n1-я ступень: 0%\n2-я ступень: 0%\n3-я ступень: 0%"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                // Календарь
                Rectangle {
                    color: Theme.surfaceColor
                    radius: Theme.radiusMedium

                    Text {
                        anchors.centerIn: parent
                        text: "📅 Календарь активности"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextPrimary
                    }
                }
            }
        }
    }
}
