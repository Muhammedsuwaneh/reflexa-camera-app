import QtQuick
import QtQuick.Controls

Rectangle {
    id: timerBox
    width: 110
    height: 40
    radius: 16
    color: "#1D293D"

    property int secondsElapsed: 0
    property bool running: false

    Timer {
        id: stopwatch
        interval: 1000
        repeat: true
        running: timerBox.running
        onTriggered: secondsElapsed++
    }

    Text {
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 17
        font.bold: true
        text: {
            let minutes = Math.floor(secondsElapsed / 60)
            let seconds = secondsElapsed % 60
            return minutes + ":" + (seconds < 10 ? "0" : "") + seconds
        }
    }

    function start() {
        reset()
        visible = true
        running = true
    }

    function stop() {
        running = false
        visible = false
    }

    function reset() {
        secondsElapsed = 0
    }
}
