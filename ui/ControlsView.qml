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

    property real currentTimerIndex: -1
    property var timers: [3, 5, 10]
    property real selectedTimerValue: 3

    signal imageCaptured()

    signal startRecording()
    signal stopRecording()

    signal timerClicked(int value)

    signal capturedClicked()

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.topMargin: -5
        radius: 12
        color: "#0b1a2a"
        visible: root.opacity > 0

        Rectangle {
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

            visible: !Camera.capturingVideo

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
                id: faceDetectorButton
                iconSource: "../assets/face.png"
                isActive: cameraControls.faceDetectorIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleFaceDetector()

                visible: !Camera.capturingVideo
            }

            CameraBehaviorButton {
                iconSource: "../assets/hdr.png"
                isActive: cameraControls.hdrIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleHDR()

                visible: !Camera.capturingVideo
            }

            Item {
                width: 60
                height: 60
                Layout.alignment: Qt.AlignVCenter
                visible: !Camera.capturingVideo

                CameraBehaviorButton {
                    id: timerButton
                    anchors.centerIn: parent
                    iconSource: "../assets/timer.png"
                    isActive: cameraControls.timerIsActive

                    onClicked:
                    {
                        if (currentTimerIndex === -1)
                        {
                            cameraControls.toggleTimer()
                            currentTimerIndex = 0
                        } else if (currentTimerIndex >= timers.length - 1)
                        {
                            cameraControls.toggleTimer()
                            currentTimerIndex = -1
                            selectedTimerValue = 0
                            timerCounter.text = "0"
                            return
                        } else
                        {
                            currentTimerIndex++
                        }

                        selectedTimerValue = root.timers[root.currentTimerIndex];
                        root.timerClicked(selectedTimerValue);
                        timerCounter.text = selectedTimerValue.toString()
                    }
                }

                Rectangle {
                    id: timerBadge
                    width: 22
                    height: 22
                    radius: 8
                    color: "#E53935"
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.topMargin: -2
                    anchors.rightMargin: -2
                    z: 10

                    Text {
                        id: timerCounter
                        anchors.centerIn: parent
                        text: "0"
                        color: "#ffffff"
                        font.pixelSize: 11
                        font.bold: true
                    }
                }
            }

            CameraControlButton {
                id: cameraButton
                iconSource: "../assets/camera.png"
                isActive: cameraControls.cameraButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked:
                {
                    if(cameraControls.cameraButtonIsActive)
                    {
                        root.imageCaptured()
                    }
                    else {

                        cameraControls.onCameraPressed()
                    }
                }

                visible: !Camera.capturingVideo
            }

            CameraControlButton {
                iconSource: "../assets/video.png"
                isActive: cameraControls.videoButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked:
                {
                    if(cameraControls.videoButtonIsActive)
                    {
                        root.startRecording();
                    }
                    else {
                        cameraControls.onVideoPressed()
                    }
                }

                visible: !Camera.capturingVideo
            }

            CameraControlButton {
                iconSource: "../assets/qr.png"
                isActive: cameraControls.qrButtonIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.onQRPressed()

                visible: !Camera.capturingVideo
            }

            CameraBehaviorButton {
                iconSource: "../assets/grid.png"
                isActive: cameraControls.gridIsActive
                Layout.alignment: Qt.AlignVCenter
                onClicked: cameraControls.toggleGrid()

                visible: !Camera.capturingVideo
            }
        }


        CameraControlButton {
            id: playButton
            anchors.centerIn: parent
            isActive: false
            iconSource: "../assets/play.png"

            visible: Camera.capturingVideo && Camera.pauseCapture

            onClicked: Camera.continueCapture()
        }

        CameraControlButton {
            id: pauseButton
            anchors.centerIn: parent
            isActive: false
            iconSource: "../assets/pause.png"

            visible: Camera.capturingVideo && !Camera.pauseCapture

            onClicked: Camera.pauseCapturing()
        }

        CameraControlButton {
            id: stopButton
            anchors.centerIn: parent
            iconSource: "../assets/stop.png"
            isActive: true
            Layout.alignment: Qt.AlignVCenter
            onClicked:
            {
                Camera.stopVideoCapture();
                root.stopRecording();
            }

            visible: false

            Connections
            {
                target: Camera
                onCapturingVideoChanged:
                {
                    stopButton.visible = Camera.capturingVideo
                }
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
            opacity: Camera.capturingVideo ? 0 : 1

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

            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    // show gallery
                    root.capturedClicked()
                }
            }
        }
    }
}
