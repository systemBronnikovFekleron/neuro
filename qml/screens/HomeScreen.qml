import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: homeScreen

    signal exerciseSelected(int exerciseIndex)
    signal showStatistics()

    Rectangle {
        anchors.fill: parent
        color: Theme.adaptiveBackground

        // Главный контент
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.paddingLarge
            spacing: Theme.paddingLarge

            // Прогресс по текущей ступени
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                color: Theme.surfaceColor
                radius: Theme.radiusMedium

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    Text {
                        text: "📊 Ваш прогресс"
                        font.pixelSize: Theme.fontSizeHeading3
                        font.weight: Theme.fontWeightMedium
                        color: Theme.adaptiveTextPrimary
                    }

                    Text {
                        text: "Подготовительная ступень"
                        font.pixelSize: Theme.fontSizeBody
                        color: Theme.adaptiveTextSecondary
                    }

                    // Прогресс-бар
                    ProgressBar {
                        Layout.fillWidth: true
                        from: 0
                        to: 100
                        value: 80  // TODO: Получать из модели

                        background: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 8
                            color: Theme.textDisabled
                            radius: 4
                        }

                        contentItem: Item {
                            implicitWidth: 200
                            implicitHeight: 8

                            Rectangle {
                                width: parent.width * (parent.parent.value / parent.parent.to)
                                height: parent.height
                                radius: 4
                                color: Theme.primaryColor
                            }
                        }
                    }

                    Row {
                        spacing: Theme.paddingMedium

                        Text {
                            text: "15 сессий"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                        }

                        Text {
                            text: "•"
                            color: Theme.adaptiveTextSecondary
                        }

                        Text {
                            text: "Средний успех: 72%"
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.adaptiveTextSecondary
                        }
                    }
                }
            }

            // Заголовок секции упражнений
            Text {
                text: "🎯 Выберите упражнение"
                font.pixelSize: Theme.fontSizeHeading2
                font.weight: Theme.fontWeightMedium
                color: Theme.adaptiveTextPrimary
            }

            // Сообщение когда нет упражнений для выбранной ступени
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 220
                color: Theme.surfaceColor
                radius: Theme.radiusMedium
                visible: exerciseModel && exerciseModel.count === 0

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 16

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "📚"
                        font.pixelSize: 64
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 500
                        text: "Упражнения для этой ступени еще не реализованы"
                        font.pixelSize: Theme.fontSizeHeading3
                        color: "#1a1a1a"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 500
                        text: "Выберите Подготовительную ступень для начала практики"
                        font.pixelSize: Theme.fontSizeBody
                        color: "#1a1a1a"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }

            // Селектор ступени
            ComboBox {
                Layout.preferredWidth: 300
                model: [
                    "Подготовительная ступень",
                    "1-я ступень: Зрение вне глаз",
                    "2-я ступень: Экран ЛБК",
                    "3-я ступень: Информационное развитие"
                ]
                currentIndex: 0

                // Явный contentItem чтобы текст был читаемым
                contentItem: Text {
                    leftPadding: 12
                    rightPadding: 40  // Место для стрелки
                    text: parent.displayText
                    font: parent.font
                    color: "#1a1a1a"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                onCurrentIndexChanged: {
                    if (exerciseModel) {
                        exerciseModel.filterByStage(currentIndex)
                    }
                }
            }

            // Сетка упражнений
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: Theme.paddingMedium

                    // GridLayout с упражнениями
                    GridLayout {
                        Layout.fillWidth: true
                        columns: 3  // Уменьшено с 4 до 3 для лучшей читаемости
                        rowSpacing: Theme.paddingMedium
                        columnSpacing: Theme.paddingMedium
                        visible: exerciseModel && exerciseModel.count > 0

                        // Repeater с данными из exerciseModel
                        Repeater {
                            model: exerciseModel ? exerciseModel.count : 0

                        delegate: Rectangle {
                            Layout.preferredWidth: 200  // Фиксированная ширина вместо fillWidth
                            Layout.preferredHeight: 240
                            color: Theme.surfaceColor
                            radius: Theme.radiusMedium

                            // Тень (отключена для Qt 6)
                            // layer.enabled: true
                            // layer.effect: DropShadow {...}

                            // Hover эффект
                            scale: mouseArea.containsMouse ? 1.02 : 1.0
                            Behavior on scale {
                                NumberAnimation {
                                    duration: Theme.animationFast
                                    easing.type: Easing.OutQuad
                                }
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.paddingMedium
                                spacing: Theme.paddingSmall

                                // Иконка - используем роль напрямую
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: model.iconEmoji || "⚡"
                                    font.pixelSize: 48
                                }

                                // ID упражнения - используем роль напрямую
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: model.stageId || ("0." + (index + 1))
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.adaptiveTextSecondary
                                }

                                // Название - используем роль напрямую
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.preferredWidth: 180
                                    text: model.name || ("Упражнение " + (index + 1))
                                    font.pixelSize: Theme.fontSizeBody
                                    font.weight: Theme.fontWeightMedium
                                    color: "#1a1a1a"
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                // Длительность - используем роль напрямую
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: (model.duration || 10) + " мин"
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.adaptiveTextSecondary
                                }

                                // Рекомендация для первого
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: index === 0 ? "⭐ Начните здесь" : ""
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.successColor
                                    visible: index === 0
                                }
                            }

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor

                                onClicked: {
                                    exerciseSelected(index)
                                }
                            }
                        }
                    }
                    }  // Конец GridLayout
                }  // Конец ColumnLayout
            }  // Конец ScrollView
        }  // Конец ColumnLayout (главный)
    }  // Конец Rectangle
}  // Конец Item
