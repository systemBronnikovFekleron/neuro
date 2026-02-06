pragma Singleton
import QtQuick 2.15

QtObject {
    id: theme

    // Текущая тема (light/dark)
    property bool darkMode: false

    // Основные цвета (Light Theme)
    readonly property color primaryColor: "#5E35B1"        // Deep Purple 600
    readonly property color primaryLight: "#9162E4"
    readonly property color primaryDark: "#280680"

    readonly property color secondaryColor: "#00ACC1"      // Cyan 600
    readonly property color secondaryLight: "#5DDEF4"
    readonly property color secondaryDark: "#007C91"

    readonly property color backgroundColor: "#FAFAFA"     // Grey 50
    readonly property color surfaceColor: "#FFFFFF"
    readonly property color errorColor: "#E53935"          // Red 600
    readonly property color successColor: "#43A047"        // Green 600
    readonly property color warningColor: "#F57C00"        // Orange 700 (темнее для лучшей читаемости белого текста)
    readonly property color infoColor: "#29B6F6"           // Light Blue 400
    readonly property color hoverColor: "#F5F5F5"          // Grey 100
    readonly property color borderColor: "#E0E0E0"         // Grey 300

    // Текст
    readonly property color textPrimary: "#212121"         // Grey 900
    readonly property color textSecondary: "#757575"       // Grey 600
    readonly property color textDisabled: "#BDBDBD"        // Grey 400
    readonly property color textOnPrimary: "#FFFFFF"
    readonly property color textOnSecondary: "#FFFFFF"

    // Метрики цвета (для графиков и индикаторов)
    readonly property color alphaColor: "#9C27B0"          // Purple
    readonly property color betaColor: "#2196F3"           // Blue
    readonly property color thetaColor: "#4CAF50"          // Green
    readonly property color concentrationColor: "#FF9800"  // Orange
    readonly property color relaxationColor: "#00BCD4"     // Cyan
    readonly property color fatigueColor: "#F44336"        // Red
    readonly property color focusColor: "#673AB7"          // Deep Purple
    readonly property color stressColor: "#FF5722"         // Deep Orange
    readonly property color heartRateColor: "#E91E63"      // Pink

    // Градиенты для графиков
    property var alphaGradient: Gradient {
        GradientStop { position: 0.0; color: "#9C27B0" }
        GradientStop { position: 1.0; color: "#E1BEE7" }
    }

    property var betaGradient: Gradient {
        GradientStop { position: 0.0; color: "#2196F3" }
        GradientStop { position: 1.0; color: "#BBDEFB" }
    }

    property var thetaGradient: Gradient {
        GradientStop { position: 0.0; color: "#4CAF50" }
        GradientStop { position: 1.0; color: "#C8E6C9" }
    }

    // Размеры и отступы
    readonly property int paddingSmall: 8
    readonly property int paddingMedium: 16
    readonly property int paddingLarge: 24
    readonly property int paddingXLarge: 32

    readonly property int radiusSmall: 4
    readonly property int radiusMedium: 8
    readonly property int radiusLarge: 12
    readonly property int radiusXLarge: 16

    // Размеры шрифтов
    readonly property int fontSizeSmall: 12
    readonly property int fontSizeBody: 16
    readonly property int fontSizeHeading3: 20
    readonly property int fontSizeHeading2: 24
    readonly property int fontSizeHeading1: 28
    readonly property int fontSizeDisplay: 32

    // Веса шрифтов
    readonly property int fontWeightRegular: Font.Normal
    readonly property int fontWeightMedium: Font.DemiBold
    readonly property int fontWeightBold: Font.Bold

    // Семейства шрифтов
    readonly property string fontFamily: "Inter"
    readonly property string fontFamilyMono: Qt.platform.os === "osx" ? "Menlo" : "Consolas"

    // Shadows (elevation)
    readonly property string shadowLight: "0px 2px 4px rgba(0, 0, 0, 0.1)"
    readonly property string shadowMedium: "0px 4px 8px rgba(0, 0, 0, 0.15)"
    readonly property string shadowHeavy: "0px 8px 16px rgba(0, 0, 0, 0.2)"

    // Длительности анимаций
    readonly property int animationFast: 150
    readonly property int animationNormal: 300
    readonly property int animationSlow: 500

    // Функция для переключения темы
    function toggleDarkMode() {
        darkMode = !darkMode
    }

    // Адаптивные цвета (меняются в зависимости от темы)
    property color adaptiveBackground: darkMode ? "#121212" : backgroundColor
    property color adaptiveSurface: darkMode ? "#1E1E1E" : surfaceColor
    property color adaptiveTextPrimary: darkMode ? "#FFFFFF" : textPrimary
    property color adaptiveTextSecondary: darkMode ? "#B0B0B0" : textSecondary
}
