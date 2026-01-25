import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property real minValue: 0
    property real maxValue: 100
    property real value: 0

    property real normalizedValue:
        maxValue > minValue
            ? (value - minValue) / (maxValue - minValue)
            : 0

    signal sliding(real value)

    RowLayout {
        spacing: 12

        Rectangle {
            id: track
            Layout.preferredWidth: 250
            height: 7
            radius: 10
            color: "#ffffff"

            Rectangle {
                id: fill
                height: parent.height
                radius: 10
                color: "#50DD60"
                width: parent.width * root.normalizedValue
            }

            Rectangle {
                id: knob
                width: 15
                height: 15
                radius: 10
                color: "#50DD60"
                anchors.verticalCenter: parent.verticalCenter

                x: Math.max(
                       0,
                       Math.min(
                           parent.width - width,
                           parent.width * root.normalizedValue - width / 2
                       )
                   )
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                preventStealing: true   // VERY important
                drag.target: knob
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: track.width - knob.width

                onPressed: updateValue(mouse.x)
                onPositionChanged: if (pressed) updateValue(mouse.x)

                function updateValue(mouseX) {
                    let clampedX = Math.max(0, Math.min(mouseX, track.width))
                    root.value =
                        root.minValue +
                        (clampedX / track.width) *
                        (root.maxValue - root.minValue)

                    root.sliding(root.value)
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: 35
            height: 30
            radius: 10
            border.color: "#ffffff"
            border.width: 1
            color: "transparent"

            Text {
                anchors.centerIn: parent
                text: Math.round(root.value)
                font.pixelSize: 14
                color: "#ffffff"
            }
        }
    }
}
