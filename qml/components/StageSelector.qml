import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Rectangle {
    id: root

    // Properties
    property int currentStage: 0  // 0=Preparatory, 1=Stage1, 2=Stage2, 3=Stage3
    property var stageNames: ["Подготовительная", "1-я ступень", "2-я ступень", "3-я ступень"]
    property var stageProgress: [80, 0, 0, 0]  // Progress in % for each stage
    property var stageUnlocked: [true, false, false, false]  // Which stages are unlocked

    // Signals
    signal stageSelected(int stage)

    implicitWidth: 800
    implicitHeight: 100
    color: "transparent"

    RowLayout {
        anchors.fill: parent
        spacing: Theme.paddingMedium

        Repeater {
            model: 4

            Rectangle {
                id: stageItem
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: Theme.radiusMedium

                property int stageIndex: index
                property bool isUnlocked: root.stageUnlocked[index]
                property bool isCurrent: root.currentStage === index
                property double progress: root.stageProgress[index]

                color: isCurrent ? Theme.primaryColor : Theme.surfaceColor
                opacity: isUnlocked ? 1.0 : 0.5

                border.width: isCurrent ? 2 : 0
                border.color: Theme.primaryColor

                Behavior on color {
                    ColorAnimation { duration: Theme.animationDurationNormal }
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: stageItem.isUnlocked
                    cursorShape: stageItem.isUnlocked ? Qt.PointingHandCursor : Qt.ForbiddenCursor
                    hoverEnabled: true

                    onEntered: {
                        if (stageItem.isUnlocked) {
                            stageItem.scale = 1.05
                        }
                    }
                    onExited: {
                        stageItem.scale = 1.0
                    }
                    onClicked: {
                        if (stageItem.isUnlocked) {
                            root.stageSelected(stageItem.stageIndex)
                        }
                    }
                }

                Behavior on scale {
                    NumberAnimation { duration: Theme.animationDurationFast }
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.paddingMedium
                    spacing: Theme.paddingSmall

                    // Stage number badge
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 32
                        height: 32
                        radius: 16
                        color: stageItem.isCurrent ? "white" : Theme.primaryColor

                        Text {
                            anchors.centerIn: parent
                            text: stageItem.stageIndex === 0 ? "П" : stageItem.stageIndex.toString()
                            font.pixelSize: Theme.fontSizeBody
                            font.weight: Theme.fontWeightBold
                            color: stageItem.isCurrent ? Theme.primaryColor : "white"
                        }
                    }

                    // Stage name
                    Text {
                        Layout.fillWidth: true
                        text: root.stageNames[stageItem.stageIndex]
                        font.pixelSize: Theme.fontSizeBody
                        font.weight: stageItem.isCurrent ? Theme.fontWeightBold : Theme.fontWeightMedium
                        color: stageItem.isCurrent ? "white" : Theme.adaptiveTextPrimary
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Item { Layout.fillHeight: true }

                    // Progress bar
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 6
                        radius: 3
                        color: stageItem.isCurrent ? Qt.rgba(1, 1, 1, 0.3) : Theme.backgroundColor

                        Rectangle {
                            height: parent.height
                            width: parent.width * (stageItem.progress / 100.0)
                            radius: parent.radius
                            color: stageItem.isCurrent ? "white" : Theme.successColor

                            Behavior on width {
                                NumberAnimation { duration: Theme.animationDurationNormal }
                            }
                        }
                    }

                    // Progress text
                    Text {
                        Layout.fillWidth: true
                        text: stageItem.progress.toFixed(0) + "%"
                        font.pixelSize: Theme.fontSizeCaption
                        font.family: Theme.fontFamilyMono
                        color: stageItem.isCurrent ? "white" : Theme.adaptiveTextSecondary
                        horizontalAlignment: Text.AlignHCenter
                    }

                    // Lock icon for locked stages
                    Text {
                        visible: !stageItem.isUnlocked
                        Layout.alignment: Qt.AlignHCenter
                        text: "🔒"
                        font.pixelSize: 24
                        opacity: 0.5
                    }
                }

                // Connector line to next stage
                Rectangle {
                    visible: index < 3
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: Theme.paddingMedium
                    height: 2
                    color: Theme.dividerColor
                }
            }
        }
    }
}
