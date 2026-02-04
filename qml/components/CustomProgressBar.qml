import QtQuick 2.15
import QtQuick.Controls 2.15
import "../styles"

Item {
    id: root

    // Properties
    property double value: 0.0
    property double from: 0.0
    property double to: 100.0
    property color progressColor: Theme.primaryColor
    property color backgroundColor: Theme.backgroundColor
    property bool showLabel: true
    property string labelSuffix: "%"
    property int barHeight: 12

    implicitWidth: 200
    implicitHeight: showLabel ? barHeight + 24 : barHeight

    // Progress value (0-1)
    readonly property double normalizedValue: (value - from) / (to - from)

    Column {
        anchors.fill: parent
        spacing: Theme.paddingSmall

        // Progress bar
        Rectangle {
            width: parent.width
            height: root.barHeight
            radius: root.barHeight / 2
            color: root.backgroundColor

            // Progress fill
            Rectangle {
                id: progressFill
                height: parent.height
                radius: parent.radius
                color: root.progressColor

                width: parent.width * Math.max(0, Math.min(1, root.normalizedValue))

                Behavior on width {
                    NumberAnimation {
                        duration: Theme.animationDurationNormal
                        easing.type: Easing.OutCubic
                    }
                }

                // Gradient overlay for visual depth
                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.rgba(1, 1, 1, 0.2) }
                        GradientStop { position: 0.5; color: "transparent" }
                        GradientStop { position: 1.0; color: Qt.rgba(0, 0, 0, 0.1) }
                    }
                }
            }

            // Shimmer effect for active progress
            Rectangle {
                visible: root.normalizedValue > 0 && root.normalizedValue < 1
                width: 30
                height: parent.height
                radius: parent.radius

                x: (progressFill.width - width) * shimmerAnimation.progress
                opacity: 0.3

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: "white" }
                    GradientStop { position: 1.0; color: "transparent" }
                }

                SequentialAnimation {
                    id: shimmerAnimation
                    running: progressFill.width > 0
                    loops: Animation.Infinite

                    property double progress: 0.0

                    NumberAnimation {
                        target: shimmerAnimation
                        property: "progress"
                        from: 0.0
                        to: 1.0
                        duration: 2000
                        easing.type: Easing.InOutQuad
                    }

                    PauseAnimation { duration: 500 }
                }
            }
        }

        // Label
        Text {
            visible: root.showLabel
            width: parent.width
            text: root.value.toFixed(0) + root.labelSuffix
            font.pixelSize: Theme.fontSizeCaption
            font.family: Theme.fontFamilyMono
            color: Theme.adaptiveTextSecondary
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
