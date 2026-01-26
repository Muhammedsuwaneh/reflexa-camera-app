import QtQuick

Item {
    id: gridOverlay
    anchors.fill: parent
    anchors.margins: 10
    z: 15
    visible: cameraControls.showGrid

    Canvas {
        anchors.fill: parent

        onPaint: {
            const ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = "rgba(255,255,255,0.35)"
            ctx.lineWidth = 1

            const w = width
            const h = height

            ctx.beginPath()
            ctx.moveTo(w / 3, 0)
            ctx.lineTo(w / 3, h)
            ctx.moveTo(2 * w / 3, 0)
            ctx.lineTo(2 * w / 3, h)

            ctx.moveTo(0, h / 3)
            ctx.lineTo(w, h / 3)
            ctx.moveTo(0, 2 * h / 3)
            ctx.lineTo(w, 2 * h / 3)

            ctx.stroke()
        }

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
    }
}
