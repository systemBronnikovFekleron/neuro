import QtQuick 2.15
import "../styles"

/**
 * ProgressBar с цветовыми зонами для метрик:
 * - Красная зона (0-40%): низкие показатели
 * - Жёлтая зона (40-70%): средние показатели
 * - Зелёная зона (70-100%): хорошие показатели
 */
Item {
    id: root

    // Основные свойства
    property double value: 0
    property double from: 0
    property double to: 100

    // Опциональный цвет (если задан, используется вместо автоматического)
    property color customColor: "transparent"
    property bool useCustomColor: customColor != "transparent" && customColor != Qt.rgba(0, 0, 0, 0)

    // Режим инверсии (для fatigue, stress - низкое значение = хорошо)
    property bool invertedMode: false

    // Размеры
    implicitWidth: 200
    implicitHeight: 8

    // Нормализованное значение (0-1)
    readonly property double normalizedValue: {
        var range = to - from
        if (range <= 0) return 0
        return Math.max(0, Math.min(1, (value - from) / range))
    }

    // Процент для определения зоны
    readonly property double percentage: normalizedValue * 100

    // Автоматический цвет по зонам
    readonly property color zoneColor: {
        var pct = invertedMode ? (100 - percentage) : percentage

        if (pct >= 70) {
            return Theme.successColor  // Зелёный
        } else if (pct >= 40) {
            return Theme.warningColor  // Жёлтый
        } else {
            return Theme.errorColor    // Красный
        }
    }

    // Итоговый цвет заполнения
    readonly property color fillColor: useCustomColor ? customColor : zoneColor

    // Фоновый контейнер с зонами
    Rectangle {
        id: background
        anchors.fill: parent
        radius: height / 2
        color: Theme.backgroundColor
        border.color: Theme.borderColor
        border.width: 1

        // Полупрозрачные зоны на фоне
        Row {
            anchors.fill: parent
            anchors.margins: 1

            // Красная зона (0-40%)
            Rectangle {
                width: parent.width * 0.4
                height: parent.height
                radius: parent.parent.radius - 1
                color: Theme.errorColor
                opacity: 0.08

                // Скругление только слева
                Rectangle {
                    anchors.right: parent.right
                    width: parent.radius
                    height: parent.height
                    color: parent.color
                    opacity: 1
                }
            }

            // Жёлтая зона (40-70%)
            Rectangle {
                width: parent.width * 0.3
                height: parent.height
                color: Theme.warningColor
                opacity: 0.08
            }

            // Зелёная зона (70-100%)
            Rectangle {
                width: parent.width * 0.3
                height: parent.height
                radius: parent.parent.radius - 1
                color: Theme.successColor
                opacity: 0.08

                // Скругление только справа
                Rectangle {
                    anchors.left: parent.left
                    width: parent.radius
                    height: parent.height
                    color: parent.color
                    opacity: 1
                }
            }
        }

        // Заполнение
        Rectangle {
            id: fill
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 1

            width: Math.max(0, (parent.width - 2) * normalizedValue)
            radius: parent.radius - 1
            color: fillColor

            // Плавная анимация ширины
            Behavior on width {
                NumberAnimation {
                    duration: Theme.animationNormal
                    easing.type: Easing.OutQuad
                }
            }

            // Плавная анимация цвета
            Behavior on color {
                ColorAnimation {
                    duration: Theme.animationNormal
                }
            }

            // Gradient overlay для глубины
            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(1, 1, 1, 0.2) }
                    GradientStop { position: 0.5; color: Qt.rgba(1, 1, 1, 0.0) }
                    GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0.1) }
                }
            }
        }

        // Разделители зон (тонкие линии)
        Rectangle {
            x: parent.width * 0.4
            y: 0
            width: 1
            height: parent.height
            color: Theme.borderColor
            opacity: 0.3
        }

        Rectangle {
            x: parent.width * 0.7
            y: 0
            width: 1
            height: parent.height
            color: Theme.borderColor
            opacity: 0.3
        }
    }
}
