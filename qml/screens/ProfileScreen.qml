import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"

Item {
    signal back()

    // Property для доступа к SessionModel и DeviceController
    property var sessionModel: null
    property var deviceController: null

    Component.onCompleted: {
        if (sessionModel) {
            sessionModel.loadUserProfile("default")
            sessionModel.loadStatistics("default")
            sessionModel.loadRecentSessions(5, "default")
        }
    }

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

                    // Управление пользователями
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 100
                        color: Theme.surfaceColor
                        radius: Theme.radiusMedium

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: Theme.paddingMedium
                            spacing: Theme.paddingSmall

                            Text {
                                text: "👥 Управление пользователями"
                                font.pixelSize: Theme.fontSizeHeading3
                                font.weight: Theme.fontWeightMedium
                                color: Theme.adaptiveTextPrimary
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: Theme.paddingMedium

                                Text {
                                    text: "Текущий:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextSecondary
                                }

                                ComboBox {
                                    id: userComboBox
                                    Layout.preferredWidth: 200

                                    model: sessionModel ? sessionModel.getAllUsers() : ["default"]

                                    Component.onCompleted: {
                                        if (sessionModel) {
                                            var users = sessionModel.getAllUsers()
                                            var currentIdx = users.indexOf(sessionModel.currentUserId())
                                            if (currentIdx >= 0) {
                                                currentIndex = currentIdx
                                            }
                                        }
                                    }

                                    onActivated: function(index) {
                                        if (sessionModel && index >= 0) {
                                            var userId = model[index]
                                            sessionModel.switchUser(userId)
                                        }
                                    }
                                }

                                Button {
                                    text: "➕ Новый"

                                    background: Rectangle {
                                        color: parent.down ? Qt.darker(Theme.successColor, 1.1) :
                                               parent.hovered ? Qt.lighter(Theme.successColor, 1.1) :
                                               Theme.successColor
                                        radius: Theme.radiusSmall
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        font: parent.font
                                        color: "white"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    onClicked: newUserDialog.open()
                                }
                            }
                        }
                    }

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
                                color: Theme.adaptiveTextPrimary
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Имя:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextSecondary
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: sessionModel ? sessionModel.userName : "Пользователь"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Дата регистрации:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextSecondary
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: sessionModel && sessionModel.firstSessionDate ? sessionModel.firstSessionDate.substring(0, 10) : "Нет данных"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Всего сессий:"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextSecondary
                                    Layout.preferredWidth: 150
                                }
                                Text {
                                    text: sessionModel ? sessionModel.totalSessions : "0"
                                    font.pixelSize: Theme.fontSizeBody
                                    color: Theme.adaptiveTextPrimary
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
                                    color: Theme.adaptiveTextPrimary
                                }

                                Item { Layout.fillWidth: true }

                                Text {
                                    text: sessionModel && sessionModel.lastCalibrationDate ?
                                          "Обновлено: " + sessionModel.lastCalibrationDate :
                                          "Калибровка не проводилась"
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.adaptiveTextSecondary
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
                                    color: Theme.adaptiveTextSecondary
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: sessionModel ? sessionModel.baselineAlpha : 0
                                }
                                Text {
                                    text: sessionModel ? sessionModel.baselineAlpha.toFixed(1) + "%" : "0%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.alphaColor
                                }

                                // Beta
                                Text {
                                    text: "Beta:"
                                    color: Theme.adaptiveTextSecondary
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: sessionModel ? sessionModel.baselineBeta : 0
                                }
                                Text {
                                    text: sessionModel ? sessionModel.baselineBeta.toFixed(1) + "%" : "0%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.betaColor
                                }

                                // Theta
                                Text {
                                    text: "Theta:"
                                    color: Theme.adaptiveTextSecondary
                                }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: sessionModel ? sessionModel.baselineTheta : 0
                                }
                                Text {
                                    text: sessionModel ? sessionModel.baselineTheta.toFixed(1) + "%" : "0%"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.thetaColor
                                }

                                // IAF
                                Text {
                                    text: "IAF:"
                                    color: Theme.adaptiveTextSecondary
                                }
                                Item { Layout.fillWidth: true }
                                Text {
                                    text: sessionModel && sessionModel.iaf > 0 ?
                                          sessionModel.iaf.toFixed(1) + " Hz" :
                                          "Не откалибровано"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.adaptiveTextPrimary
                                }

                                // Heart Rate
                                Text {
                                    text: "Пульс (покой):"
                                    color: Theme.adaptiveTextSecondary
                                }
                                Item { Layout.fillWidth: true }
                                Text {
                                    text: sessionModel && sessionModel.baselineHeartRate > 0 ?
                                          Math.round(sessionModel.baselineHeartRate) + " BPM" :
                                          "Нет данных"
                                    font.family: Theme.fontFamilyMono
                                    color: Theme.heartRateColor
                                }
                            }

                            Item { Layout.fillHeight: true }

                            RowLayout {
                                Layout.alignment: Qt.AlignHCenter
                                spacing: Theme.paddingMedium

                                Button {
                                    text: deviceController && deviceController.isSessionActive ?
                                          "🔄 Повторить калибровку" :
                                          (deviceController && deviceController.isConnected ?
                                           "▶️ Запустить сессию" : "📡 Подключить устройство")

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
                                        if (!deviceController) {
                                            console.log("DeviceController не доступен")
                                            return
                                        }

                                        if (deviceController.isSessionActive) {
                                            // Запускаем калибровку
                                            deviceController.startCalibration()
                                        } else if (deviceController.isConnected) {
                                            // Запускаем сессию
                                            deviceController.startSession()
                                        } else {
                                            // Ищем устройства
                                            deviceController.discoverDevices()
                                        }
                                    }
                                }

                                // Статус подключения
                                Text {
                                    text: {
                                        if (!deviceController) return "❓"
                                        if (deviceController.isSessionActive) return "✓ Сессия активна"
                                        if (deviceController.isConnected) return "✓ Подключено"
                                        return "⚠ Не подключено"
                                    }
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: {
                                        if (!deviceController) return Theme.adaptiveTextSecondary
                                        if (deviceController.isSessionActive) return Theme.successColor
                                        if (deviceController.isConnected) return Theme.warningColor
                                        return Theme.errorColor
                                    }
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
                                color: Theme.adaptiveTextPrimary
                            }

                            ListView {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 8
                                clip: true

                                model: sessionModel ? sessionModel.recentSessions : []

                                delegate: Rectangle {
                                    width: ListView.view.width
                                    height: 30
                                    color: "transparent"

                                    RowLayout {
                                        anchors.fill: parent
                                        spacing: Theme.paddingMedium

                                        Text {
                                            text: modelData.timestamp ? modelData.timestamp.substring(0, 10) : ""
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.adaptiveTextSecondary
                                            Layout.preferredWidth: 100
                                        }

                                        Text {
                                            text: modelData.exerciseName || ""
                                            font.pixelSize: Theme.fontSizeSmall
                                            color: Theme.adaptiveTextPrimary
                                            Layout.fillWidth: true
                                        }

                                        Text {
                                            text: modelData.successRate ? modelData.successRate.toFixed(0) + "%" : "0%"
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

    // Диалог создания нового пользователя
    Dialog {
        id: newUserDialog
        title: "Новый пользователь"
        modal: true
        anchors.centerIn: parent
        width: 350

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.paddingMedium

            Text {
                text: "Введите ID и имя нового пользователя:"
                font.pixelSize: Theme.fontSizeBody
                color: Theme.adaptiveTextPrimary
            }

            TextField {
                id: newUserIdField
                Layout.fillWidth: true
                placeholderText: "ID пользователя (латиница)"
                validator: RegularExpressionValidator { regularExpression: /[a-zA-Z0-9_]+/ }
            }

            TextField {
                id: newUserNameField
                Layout.fillWidth: true
                placeholderText: "Имя пользователя"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.paddingMedium

                Button {
                    Layout.fillWidth: true
                    text: "Отмена"

                    background: Rectangle {
                        color: parent.down ? Qt.darker(Theme.textDisabled, 1.1) :
                               parent.hovered ? Qt.lighter(Theme.textDisabled, 1.1) :
                               Theme.textDisabled
                        radius: Theme.radiusSmall
                    }

                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: Theme.textPrimary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: newUserDialog.close()
                }

                Button {
                    Layout.fillWidth: true
                    text: "Создать"
                    enabled: newUserIdField.text.length > 0

                    background: Rectangle {
                        color: parent.enabled ?
                               (parent.down ? Qt.darker(Theme.successColor, 1.1) :
                                parent.hovered ? Qt.lighter(Theme.successColor, 1.1) :
                                Theme.successColor) :
                               Theme.textDisabled
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
                        if (sessionModel && newUserIdField.text.length > 0) {
                            var success = sessionModel.createUser(newUserIdField.text, newUserNameField.text)
                            if (success) {
                                sessionModel.switchUser(newUserIdField.text)
                                // Обновляем список пользователей в ComboBox
                                userComboBox.model = sessionModel.getAllUsers()
                                var users = sessionModel.getAllUsers()
                                var idx = users.indexOf(newUserIdField.text)
                                if (idx >= 0) {
                                    userComboBox.currentIndex = idx
                                }
                                newUserIdField.text = ""
                                newUserNameField.text = ""
                                newUserDialog.close()
                            }
                        }
                    }
                }
            }
        }
    }
}
