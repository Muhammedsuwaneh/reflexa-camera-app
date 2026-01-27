import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 1280
    height: 900
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Window
    color: "transparent"

    property real countDownTime: 0
    property int currentCount: 0

    property bool pendingPhoto: false
    property bool pendingVideo: false


    GridLayout {
        rows: 3
        columns: 3
        rowSpacing: 0
        columnSpacing: 0
        anchors.fill: parent

        TitleBar {
            Layout.row: 0
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.preferredHeight: 45
        }

        Rectangle {
            id: captureContainer
            Layout.row: 1
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000"
            z: 10

            Image {
                id: cameraView
                anchors.fill: parent
                anchors.margins: 10
                fillMode: Image.PreserveAspectCrop
                cache: false
                smooth: true
                source: "image://camera/live"

                transform: Scale {
                                origin.x: previewer.width / 2
                                origin.y: previewer.height / 2
                                xScale: ImageTextureController.zoomFactor
                                yScale: ImageTextureController.zoomFactor
                            }

                Connections {
                    target: CameraController

                    function onFrameCleared() {
                        cameraView.source = ""
                    }

                    function onFrameChanged() {
                        cameraView.source = "image://camera/live?" + Date.now()
                    }
                }
            }

            Item {
                id: countdownOverlay
                z: 20
                visible: false
                anchors.centerIn: parent

                Timer {
                    id: countDownTimer
                    interval: 1000
                    repeat: true

                    onTriggered: {
                        currentCount--

                        if (currentCount <= 0) {
                            stop()
                            countdownOverlay.visible = false

                            if (pendingPhoto) {
                                flash.trigger()
                                CameraController.takeShot()
                            }

                            if (pendingVideo) {
                                CameraController.startVideoCapture()
                                recordTimer.start()
                            }

                            pendingPhoto = false
                            pendingVideo = false
                        }
                    }
                }

                function startCountdown(seconds) {
                    currentCount = seconds
                    visible = true
                    countDownTimer.start()
                }

                Text {
                    text: currentCount
                    font.pixelSize: 150
                    font.bold: true
                    color: "#ffffff"
                    anchors.centerIn: parent
                }
            }


            // FLASH
            Rectangle {
                id: flash
                anchors.fill: parent
                anchors.margins: 10
                radius: 8
                color: "#ffffff"
                opacity: 0
                visible: opacity > 0
                z: 20

                Behavior on opacity {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.OutQuad
                    }
                }

                Timer {
                    id: flashTimer
                    interval: 300
                    repeat: false
                    onTriggered: flash.opacity = 0
                }

                function trigger() {
                    flash.opacity = 1
                    flashTimer.restart()
                }
            }

            GridLines
            {
                id: gridLines
                anchors.fill: parent
                visible: true

                Connections
                {
                    target: cameraControls
                    onGridIsActiveChanged:
                    {
                        gridLines.visible = cameraControls.gridIsActive ? 1 : 0
                    }
                }

                Behavior on opacity { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
            }
        }

        RecordTimer
        {
            id: recordTimer

              Layout.row: 1
              Layout.columnSpan: 3
              Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
              Layout.bottomMargin: 20

              z: 25
              opacity: 0

            Connections
            {
                target: CameraController
                onCapturingVideoChanged:
                {
                    recordTimer.opacity = CameraController.capturingVideo ? 1 : 0
                }
            }

            Behavior on opacity { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        }

        SettingsView {
            id: settingsView
            Layout.row: 1
            Layout.column: 0
            Layout.fillHeight: true
            Layout.preferredWidth: 350
            z: 10
            opacity: 0

            Connections
            {
                target: cameraControls
                onShowSettingsChanged:
                {
                    settingsView.opacity = cameraControls.showSettings ? 1 : 0
                }
            }

            Behavior on opacity { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        }


        FiltersView {
            id: filtersView
            Layout.row: 1
            Layout.column: 2
            Layout.fillHeight: true
            Layout.preferredWidth: 350
            z: 20
            opacity: 0

            Connections
            {
                target: cameraControls
                onShowFiltersChanged:
                {
                    filtersView.opacity = cameraControls.showFilters ? 1 : 0
                }
            }

            Behavior on opacity { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        }

        ControlsView {
            id: controlsView
            Layout.row: 2
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            z: 20

            onStartRecording: {
                if (countDownTime > 0) {
                    pendingVideo = true
                    countdownOverlay.startCountdown(countDownTime)
                } else {
                    CameraController.startVideoCapture()
                    recordTimer.start()
                }

                if(cameraControls.showFilters)
                {
                    cameraControls.toggleFilters()
                }

                if(cameraControls.showSettings)
                {
                    cameraControls.toggleSettings()
                }
            }

            onStopRecording:
            {
                recordTimer.stop()
            }

            onImageCaptured: {
                if (countDownTime > 0) {
                    pendingPhoto = true
                    countdownOverlay.startCountdown(countDownTime)
                } else {
                    flash.trigger()
                    CameraController.takeShot()
                }
            }

            onTimerClicked: (value) =>
            {
                countDownTime = value
            }

            onCapturedClicked: () =>
            {
                if(!recordingSpinner.visible)
                {
                    // show gallery control buttons
                    controlsView.opacity = 0
                    captureContainer.opacity = 0
                    galleryView.opacity = 1
                    settingsView.opacity = 0
                    filtersView.opacity = 0
                    filtersButton.opacity = 0
                }
            }
        }
    }


    GalleryView {
        id: galleryView

        anchors {
            top: parent.top
            topMargin: 45
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        z: 120
        opacity: 0
        visible: opacity > 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutCubic
            }
        }

        onReturnClicked:
        {
            controlsView.opacity = 1
            captureContainer.opacity = 1
            galleryView.opacity = 0
            filtersButton.opacity = 1
        }
    }

    AnimatedImage
    {
        id: recordingSpinner
        source: "../assets/recording.gif"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 30
        anchors.topMargin: 80
        width: 50
        height: 30
        playing: true

        visible: false

        Connections
        {
            target: CameraController
            onCapturingVideoChanged:
            {
                recordingSpinner.visible = CameraController.capturingVideo
            }
        }
    }

    Button
    {
        id: filtersButton
        width: 60
        height: 28
        flat: true
        z: 30

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.bottomMargin: 120

        property color backgroundColor: "#808080"

        background: Rectangle {
            radius: 10
            color: backgroundColor
            opacity: buttonMouseArea.containsMouse ? 0.75 : 1
        }

        contentItem: Text {
            text: "FILTERS"
            color: "#333"
            font.pixelSize: 12
            anchors.centerIn: parent
        }


        visible: true

        Connections
        {
            target: CameraController
            onCapturingVideoChanged:
            {
                filtersButton.visible = !CameraController.capturingVideo
            }
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: cameraControls.toggleFilters()
        }

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }
}
