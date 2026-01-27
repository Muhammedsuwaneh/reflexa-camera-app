import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Item {
    id: root
    clip: true

    signal returnClicked()

    property string mediaType: "photo"
    property bool mediaExist: false

    property real zoomFactor: 1.0
    property real minZoom: 0.6
    property real maxZoom: 3.0
    property real rotationAngle: 0

    Connections {
        target: CameraController
        function onCurrentMediaTypeChanged()
        {
            root.mediaType = CameraController.currentMediaType

            if (root.mediaType === "photo") {
                player.stop()
            }

            console.log("captured: " + CameraController.currentMediaType)
        }

        function onRecentCapturedChanged()
        {
            if (CameraController.recentCaptured !== null)
            {
                root.mediaExist = true

                if (root.mediaType === "photo")
                {
                    recentPhoto.source = "image://captured/current?" + Date.now()
                }

                console.log("captured: " + CameraController.currentMediaType)
            }
        }
    }

    Rectangle {
        id: roundedBackground
        anchors.fill: parent
        radius: 18
        color: "#000000"

        Image {
            id: recentPhoto
            anchors.fill: parent
            anchors.margins: 10

            fillMode: Image.PreserveAspectFit
            smooth: true
            cache: false
            opacity: mediaType === "photo" ? 1 : 0

            transform: [
                Scale {
                    origin.x: recentPhoto.width / 2
                    origin.y: recentPhoto.height / 2
                    xScale: zoomFactor
                    yScale: zoomFactor
                },
                Rotation {
                    origin.x: recentPhoto.width / 2
                    origin.y: recentPhoto.height / 2
                    angle: rotationAngle
                }
            ]

            Behavior on opacity {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }

        MediaPlayer {
            id: player
            source: "../assets/test.mp4"
            audioOutput: AudioOutput {}
            videoOutput: videoOutput
        }

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            anchors
            {
                leftMargin: 60
                rightMargin: 60
                topMargin: 20
                bottomMargin: 20
            }

            opacity: root.mediaType === "photo" ? 0 : 1

            Behavior on opacity {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }
    }

    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 20
        color: "#000000"
    }

    Text {
        anchors.centerIn: parent
        text: "No media yet"
        color: "#888"
        font.pixelSize: 18
        visible: root.mediaExist ? false : true
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 25
        width: 50
        height: 50
        source: "../assets/camera.png"
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: root.returnClicked()
        }
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 40
        height: 40
        source: "../assets/arrow_left.png"
        visible: root.mediaExist

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: console.log("Previous media")
        }
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        width: 40
        height: 40
        source: "../assets/arrow_right.png"
        visible: root.mediaExist

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: console.log("Next media")
        }
    }

    Rectangle {
        id: galleryControls
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50

        width: 300
        height: 60
        radius: 20
        color: "#F9FAFB"
        opacity: 0.85
        visible: mediaExist

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 30

            Item { Layout.fillWidth: true }

            Image {
                width: 22
                height: 22
                source: "../assets/delete.png"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: console.log("Delete media")
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/rotate.png"
                visible: root.mediaType === "photo"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: rotationAngle = (rotationAngle + 90) % 360
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/zoom_in.png"
                visible: root.mediaType === "photo"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: zoomFactor = Math.min(maxZoom, zoomFactor + 0.2)
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/zoom_out.png"
                visible: mediaType === "photo"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: zoomFactor = Math.max(minZoom, zoomFactor - 0.2)
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/play.png"
                visible: root.mediaType !== "photo" &&
                         player.playbackState !== MediaPlayer.PlayingState

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: player.play()
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/pause.png"
                visible: player.playbackState === MediaPlayer.PlayingState

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: player.pause()
                }
            }

            Item { Layout.fillWidth: true }
        }
    }

    Behavior on zoomFactor {
        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
    }

    Behavior on rotationAngle {
        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
    }
}
