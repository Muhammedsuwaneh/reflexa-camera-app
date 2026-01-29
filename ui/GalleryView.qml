import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Item {
    id: root
    clip: true

    property string mediaType: "photo"
    property bool mediaExist: false

    property real zoomFactor: 1.0
    property real minZoom: 0.6
    property real maxZoom: 3.0
    property real rotationAngle: 0

    property int currentIndex: 0

    signal returnClicked()

    Connections {
        target: CameraController
        function onCurrentMediaTypeChanged()
        {
            root.mediaType = CameraController.currentMediaType

            if (root.mediaType === "photo") {
                player.stop()
            }
        }
    }

    Connections {
        target: CameraController.mediaModel

        function onCountChanged() {
            if (CameraController.mediaModel.count <= 0)
                return

            currentIndex = CameraController.mediaModel.count - 1
            var item = CameraController.mediaModel.get(currentIndex)

            mediaType = item.type
            mediaExist = true

            if (item.type === "photo") {
                recentPhoto.source = "file:///" + item.filePath
                player.stop()
            } else {
                player.stop()
                player.source = "file:///" + item.filePath
            }
        }
    }

    Rectangle {
        id: roundedBackground
        anchors.fill: parent
        radius: 18
        color: "#000000"

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            visible: root.mediaType !== "photo"
            z: 2
            fillMode: VideoOutput.PreserveAspectFit
        }

        MediaPlayer {
            id: player
            videoOutput: videoOutput
            audioOutput: AudioOutput {}
        }

        Image {
            id: recentPhoto
            anchors.fill: parent
            anchors.margins: 10
            visible: root.mediaType === "photo"
            z: 1

            fillMode: Image.PreserveAspectFit
            smooth: true
            cache: false

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
            onClicked:
            {
                if (currentIndex <= 0) return
                   currentIndex--

                    var item = CameraController.mediaModel.get(currentIndex)
                    root.mediaType = item.type

                   if (item.type === "photo") {
                       recentPhoto.source = "file:///" + item.filePath
                       player.stop()
                   } else {
                       player.source = "file:///" + item.filePath
                       player.play()
                }
            }
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
            onClicked:
            {
                if (currentIndex >= CameraController.mediaModel.count - 1) return
                   currentIndex++

                   var item = CameraController.mediaModel.get(currentIndex)
                   root.mediaType = item.type

                   if (item.type === "photo") {
                       recentPhoto.source = "file:///" + item.filePath
                       player.stop()
                   } else {
                       player.source = "file:///" + item.filePath
                       player.play()
                }
            }
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
                    onClicked: CameraController.deleteFile(currentIndex)
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

    Component.onCompleted: {
        if (CameraController.mediaModel &&
            CameraController.mediaModel.count > 0) {

            currentIndex = CameraController.mediaModel.count - 1
            var item = CameraController.mediaModel.get(currentIndex)

            mediaExist = true
            mediaType = item.type

            if (item.type === "photo") {
                recentPhoto.source = "file:///" + item.filePath
                player.stop()
            } else {
                player.stop()
                player.source = "file:///" + item.filePath
            }
        }
    }
}
