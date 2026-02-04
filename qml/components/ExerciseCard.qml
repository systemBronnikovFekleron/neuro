import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Rectangle {
    id: root

    // Properties
    property string exerciseName: "Упражнение"
    property string exerciseId: "0.1"
    property int duration: 10
    property bool isCompleted: false
    property double successRate: 0.0
    property string iconEmoji: "⚡"

    // Signals
    signal clicked()

    width: 280
    height: 160
    radius: Theme.radiusMedium
    color: Theme.surfaceColor

    border.width: hovered ? 2 : 0
    border.color: Theme.primaryColor

    // Hover state
    property bool hovered: false

    // Hover animation
    Behavior on border.width {
        NumberAnimation { duration: Theme.animationDurationFast }
    }

    Behavior on scale {
        NumberAnimation { duration: Theme.animationDurationFast }
    }

    scale: hovered ? 1.02 : 1.0

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: root.hovered = true
        onExited: root.hovered = false
        onClicked: root.clicked()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.paddingMedium
        spacing: Theme.paddingSmall

        // Header with ID and completion badge
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: root.exerciseId
                font.pixelSize: Theme.fontSizeCaption
                font.family: Theme.fontFamilyMono
                font.weight: Theme.fontWeightMedium
                color: Theme.primaryColor
            }

            Item { Layout.fillWidth: true }

            // Completion badge
            Rectangle {
                visible: root.isCompleted
                width: 24
                height: 24
                radius: 12
                color: Theme.successColor

                Text {
                    anchors.centerIn: parent
                    text: "✓"
                    font.pixelSize: 14
                    color: "white"
                }
            }
        }

        // Icon
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.iconEmoji
            font.pixelSize: 48
        }

        // Exercise name
        Text {
            Layout.fillWidth: true
            text: root.exerciseName
            font.pixelSize: Theme.fontSizeBody
            font.weight: Theme.fontWeightMedium
            color: Theme.adaptiveTextPrimary
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }

        Item { Layout.fillHeight: true }

        // Footer with duration and success rate
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: root.duration + " мин"
                font.pixelSize: Theme.fontSizeCaption
                color: Theme.adaptiveTextSecondary
            }

            Item { Layout.fillWidth: true }

            Text {
                visible: root.isCompleted
                text: root.successRate.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeCaption
                font.family: Theme.fontFamilyMono
                color: root.successRate >= 70 ? Theme.successColor : Theme.warningColor
            }
        }
    }

    // Hover overlay
    Rectangle {
        anchors.fill: parent
        radius: Theme.radiusMedium
        color: Theme.primaryColor
        opacity: hovered ? 0.05 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: Theme.animationDurationFast }
        }
    }
}
