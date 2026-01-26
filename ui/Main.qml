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
            Layout.row: 1
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000"
            z: 0

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
                    target: Camera

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

                        timeText.text = currentCount.toString()

                        if (currentCount <= 0) {
                            stop()
                            countdownOverlay.visible = false
                            flash.trigger()
                        }
                    }
                }

                function trigger() {
                    currentCount = countDownTime
                    timeText.text = currentCount.toString()
                    visible = true
                    countDownTimer.start()
                }

                Text {
                    id: timeText
                    text: currentCount.toString()
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
                target: Camera
                onCapturingVideoChanged:
                {
                    if(countDownTime == 0)
                        recordTimer.opacity = Camera.capturingVideo ? 1 : 0
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
            z: 10
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

            onStartRecording: recordTimer.start()
            onStopRecording:  recordTimer.stop()

            onImageCaptured: {
                if(countDownTime == 0)
                    flash.trigger()
                else
                    countdownOverlay.trigger()
            }

            onTimerClicked: (value) =>
            {
                countDownTime = value
            }
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
        width: 70
        height: 50
        playing: true

        visible: false

        Connections
        {
            target: Camera
            onCapturingVideoChanged:
            {
                recordingSpinner.visible = Camera.capturingVideo
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
            target: Camera
            onCapturingVideoChanged:
            {
                filtersButton.visible = !Camera.capturingVideo
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
