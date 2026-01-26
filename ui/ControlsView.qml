import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    radius: 18
    clip: true
    color: "transparent"
    height: 100
    width: parent.width

    signal imageCaptured()

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.topMargin: -5
        radius: 12
        color: "#0b1a2a"

        Rectangle {
            id: bg
            anchors.leftMargin: 30
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 40
            height: 40
            radius: width / 2
            border.color: "#fff"
            border.width: 2
            antialiasing: true
            color: "transparent"

            Rectangle {
                anchors.centerIn: parent
                width: parent.width
                height: parent.height
                radius: width / 2
                color: "transparent"
            }

            Image {
                anchors.centerIn: parent
                width: 20
                height: 20
                source: "../assets/settings.png"
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: cameraControls.toggleSettings()
            }
        }

        RowLayout {
            id: controlsRow
            anchors.centerIn: parent
            spacing: 10

            CameraBehaviorButton {
                iconSource: "../assets/face.png"
                isActive: cameraControls.faceDetectorIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleFaceDetector()
            }

            CameraBehaviorButton {
                iconSource: "../assets/hdr.png"
                isActive: cameraControls.hdrIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleHDR()
            }

            CameraBehaviorButton {
                iconSource: "../assets/timer.png"
                isActive: cameraControls.timerIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleTimer()
            }

            CameraControlButton {
                iconSource: "../assets/camera.png"
                isActive: cameraControls.cameraButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked:
                {
                    cameraControls.onCameraPressed()
                    root.imageCaptured()
                }
            }

            CameraControlButton {
                iconSource: "../assets/video.png"
                isActive: cameraControls.videoButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.onVideoPressed()
            }

            CameraControlButton {
                iconSource: "../assets/qr.png"
                isActive: cameraControls.qrButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.onQRPressed()
            }

            CameraBehaviorButton {
                iconSource: "../assets/grid.png"
                isActive: cameraControls.gridIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleGrid()
            }
        }

        // RECENT PHOTO
        Rectangle {
            anchors.rightMargin: 30
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: 65
            height: 60
            radius: 10
            border.color: "#fff"
            border.width: 2
            antialiasing: true
            color: "#18181B"

            Image
            {
                id: recentPhotoView
                width: 60
                height: 60
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: "image://captured/current"

                Connections {
                    target: Camera

                    function onRecentCapturedChanged() {
                        recentPhotoView.source = "image://captured/current?" + Date.now()
                    }
                }
            }
        }
    }
}
