import QtQuick 2.15
import "../styles"

/**
 * Canvas-based линейный график для отображения истории Alpha/Beta/Theta.
 * Использует Canvas вместо QtCharts для избежания краша в Qt 6.10.1.
 */
Canvas {
    id: chart

    // Данные для графиков (массивы значений 0-100)
    property var alphaHistory: []
    property var betaHistory: []
    property var thetaHistory: []

    // Максимальное значение для масштабирования
    property real maxValue: 100

    // Отступы
    property int paddingLeft: 45
    property int paddingRight: 15
    property int paddingTop: 15
    property int paddingBottom: 30

    // Толщина линий
    property int lineWidth: 2

    // Перерисовка при изменении данных
    onAlphaHistoryChanged: requestPaint()
    onBetaHistoryChanged: requestPaint()
    onThetaHistoryChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)

        // Область графика
        var graphWidth = width - paddingLeft - paddingRight
        var graphHeight = height - paddingTop - paddingBottom

        if (graphWidth <= 0 || graphHeight <= 0) return

        // Фон
        ctx.fillStyle = Theme.backgroundColor
        ctx.fillRect(0, 0, width, height)

        // Рисуем сетку
        drawGrid(ctx, graphWidth, graphHeight)

        // Рисуем линии графиков
        if (alphaHistory && alphaHistory.length > 1) {
            drawLine(ctx, alphaHistory, Theme.alphaColor, graphWidth, graphHeight)
        }
        if (betaHistory && betaHistory.length > 1) {
            drawLine(ctx, betaHistory, Theme.betaColor, graphWidth, graphHeight)
        }
        if (thetaHistory && thetaHistory.length > 1) {
            drawLine(ctx, thetaHistory, Theme.thetaColor, graphWidth, graphHeight)
        }

        // Рисуем легенду
        drawLegend(ctx)

        // Рисуем оси
        drawAxes(ctx, graphWidth, graphHeight)
    }

    function drawGrid(ctx, graphWidth, graphHeight) {
        ctx.strokeStyle = Theme.borderColor
        ctx.lineWidth = 1
        ctx.globalAlpha = 0.3

        // Горизонтальные линии (каждые 25%)
        for (var i = 0; i <= 4; i++) {
            var y = paddingTop + graphHeight * (i / 4)
            ctx.beginPath()
            ctx.moveTo(paddingLeft, y)
            ctx.lineTo(paddingLeft + graphWidth, y)
            ctx.stroke()
        }

        // Вертикальные линии (каждые 30 сек = 30 точек при 1 сек/точка)
        var numPoints = Math.max(alphaHistory.length, betaHistory.length, thetaHistory.length)
        if (numPoints < 2) numPoints = 120  // По умолчанию 2 минуты

        var interval = 30  // 30 секунд
        for (var j = 0; j <= numPoints; j += interval) {
            var x = paddingLeft + graphWidth * (j / numPoints)
            ctx.beginPath()
            ctx.moveTo(x, paddingTop)
            ctx.lineTo(x, paddingTop + graphHeight)
            ctx.stroke()
        }

        ctx.globalAlpha = 1.0
    }

    function drawLine(ctx, data, color, graphWidth, graphHeight) {
        if (!data || data.length < 2) return

        ctx.strokeStyle = color
        ctx.lineWidth = lineWidth
        ctx.lineJoin = "round"
        ctx.lineCap = "round"
        ctx.globalAlpha = 0.9

        ctx.beginPath()

        var numPoints = data.length
        for (var i = 0; i < numPoints; i++) {
            var x = paddingLeft + graphWidth * (i / (numPoints - 1))
            var value = Math.min(Math.max(data[i], 0), maxValue)
            var y = paddingTop + graphHeight * (1 - value / maxValue)

            if (i === 0) {
                ctx.moveTo(x, y)
            } else {
                ctx.lineTo(x, y)
            }
        }

        ctx.stroke()
        ctx.globalAlpha = 1.0

        // Рисуем полупрозрачную заливку под линией
        ctx.globalAlpha = 0.1
        ctx.fillStyle = color
        ctx.lineTo(paddingLeft + graphWidth, paddingTop + graphHeight)
        ctx.lineTo(paddingLeft, paddingTop + graphHeight)
        ctx.closePath()
        ctx.fill()
        ctx.globalAlpha = 1.0
    }

    function drawLegend(ctx) {
        var legendY = paddingTop / 2 + 5
        var legendX = paddingLeft + 10
        var spacing = 80

        ctx.font = "12px " + Theme.fontFamily

        // Alpha
        ctx.fillStyle = Theme.alphaColor
        ctx.fillRect(legendX, legendY - 8, 16, 3)
        ctx.fillText("Alpha", legendX + 20, legendY)

        // Beta
        ctx.fillStyle = Theme.betaColor
        ctx.fillRect(legendX + spacing, legendY - 8, 16, 3)
        ctx.fillText("Beta", legendX + spacing + 20, legendY)

        // Theta
        ctx.fillStyle = Theme.thetaColor
        ctx.fillRect(legendX + spacing * 2, legendY - 8, 16, 3)
        ctx.fillText("Theta", legendX + spacing * 2 + 20, legendY)
    }

    function drawAxes(ctx, graphWidth, graphHeight) {
        ctx.strokeStyle = Theme.adaptiveTextSecondary
        ctx.fillStyle = Theme.adaptiveTextSecondary
        ctx.lineWidth = 1
        ctx.font = "11px " + Theme.fontFamilyMono

        // Ось Y (значения)
        ctx.beginPath()
        ctx.moveTo(paddingLeft, paddingTop)
        ctx.lineTo(paddingLeft, paddingTop + graphHeight)
        ctx.stroke()

        // Метки Y
        for (var i = 0; i <= 4; i++) {
            var y = paddingTop + graphHeight * (i / 4)
            var value = Math.round(maxValue * (1 - i / 4))
            ctx.fillText(value + "%", 5, y + 4)
        }

        // Ось X (время)
        ctx.beginPath()
        ctx.moveTo(paddingLeft, paddingTop + graphHeight)
        ctx.lineTo(paddingLeft + graphWidth, paddingTop + graphHeight)
        ctx.stroke()

        // Метки X (время в секундах)
        var numPoints = Math.max(alphaHistory.length, betaHistory.length, thetaHistory.length)
        if (numPoints < 2) numPoints = 120

        // Показываем метки каждые 30 сек
        var interval = 30
        for (var j = 0; j <= numPoints; j += interval) {
            var x = paddingLeft + graphWidth * (j / numPoints)
            var timeLabel = Math.round(j) + "s"
            ctx.fillText(timeLabel, x - 10, paddingTop + graphHeight + 15)
        }
    }
}
