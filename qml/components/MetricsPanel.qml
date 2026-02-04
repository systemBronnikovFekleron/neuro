import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Rectangle {
    id: root

    // Properties for real-time metrics
    property double alphaValue: 0.0
    property double betaValue: 0.0
    property double thetaValue: 0.0
    property double concentrationValue: 0.0
    property double relaxationValue: 0.0
    property double fatigueValue: 0.0
    property int heartRate: 0
    property double successRate: 0.0

    // Visual properties
    color: Theme.surfaceColor
    radius: Theme.radiusMedium

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.paddingMedium
        spacing: Theme.paddingSmall

        // Header
        Text {
            text: "📊 Текущие метрики"
            font.pixelSize: Theme.fontSizeHeading3
            font.weight: Theme.fontWeightMedium
            color: Theme.adaptiveTextPrimary
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.dividerColor
        }

        // Brain waves section
        GridLayout {
            Layout.fillWidth: true
            columns: 3
            rowSpacing: Theme.paddingSmall
            columnSpacing: Theme.paddingMedium

            // Alpha
            Text {
                text: "Alpha"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.alphaValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.alphaColor
                }
            }
            Text {
                text: root.alphaValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.alphaColor
                Layout.preferredWidth: 50
            }

            // Beta
            Text {
                text: "Beta"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.betaValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.betaColor
                }
            }
            Text {
                text: root.betaValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.betaColor
                Layout.preferredWidth: 50
            }

            // Theta
            Text {
                text: "Theta"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.thetaValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.thetaColor
                }
            }
            Text {
                text: root.thetaValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.thetaColor
                Layout.preferredWidth: 50
            }
        }

        Item { height: Theme.paddingSmall }

        // Productivity metrics section
        GridLayout {
            Layout.fillWidth: true
            columns: 3
            rowSpacing: Theme.paddingSmall
            columnSpacing: Theme.paddingMedium

            // Concentration
            Text {
                text: "Концентрация"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.concentrationValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.concentrationColor
                }
            }
            Text {
                text: root.concentrationValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.concentrationColor
                Layout.preferredWidth: 50
            }

            // Relaxation
            Text {
                text: "Релаксация"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.relaxationValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.relaxationColor
                }
            }
            Text {
                text: root.relaxationValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.relaxationColor
                Layout.preferredWidth: 50
            }

            // Fatigue
            Text {
                text: "Усталость"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextSecondary
            }
            ProgressBar {
                Layout.fillWidth: true
                from: 0; to: 100
                value: root.fatigueValue

                background: Rectangle {
                    implicitHeight: 8
                    color: Theme.backgroundColor
                    radius: 4
                }

                contentItem: Rectangle {
                    implicitHeight: 8
                    radius: 4
                    color: Theme.errorColor
                }
            }
            Text {
                text: root.fatigueValue.toFixed(0) + "%"
                font.pixelSize: Theme.fontSizeBody
                font.family: Theme.fontFamilyMono
                color: Theme.errorColor
                Layout.preferredWidth: 50
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Theme.dividerColor
        }

        // Heart Rate and Success Rate
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.paddingLarge

            // Heart Rate
            RowLayout {
                spacing: Theme.paddingSmall

                Text {
                    text: "❤️ Пульс"
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.adaptiveTextSecondary
                }

                Text {
                    text: root.heartRate + " BPM"
                    font.pixelSize: Theme.fontSizeHeading3
                    font.family: Theme.fontFamilyMono
                    font.weight: Theme.fontWeightBold
                    color: Theme.heartRateColor
                }
            }

            Item { Layout.fillWidth: true }

            // Success Rate
            RowLayout {
                spacing: Theme.paddingSmall

                Text {
                    text: "Success Rate"
                    font.pixelSize: Theme.fontSizeBody
                    color: Theme.adaptiveTextSecondary
                }

                Text {
                    text: root.successRate.toFixed(0) + "%"
                    font.pixelSize: Theme.fontSizeHeading3
                    font.family: Theme.fontFamilyMono
                    font.weight: Theme.fontWeightBold
                    color: root.successRate >= 70 ? Theme.successColor : Theme.warningColor
                }
            }
        }
    }
}
