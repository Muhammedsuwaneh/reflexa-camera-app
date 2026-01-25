import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    clip: true
    color: "transparent"
    width: parent.width

    Rectangle {
        anchors.fill: parent
        color: "#111111"

        GridLayout
        {
            anchors.topMargin: 15
            rows: 3
            rowSpacing: 15

            // CAMS
            ColumnLayout {
                Layout.row: 0
                Layout.fillWidth: true
                spacing: 6
                Layout.topMargin: 15
                Layout.leftMargin: 20

                Text {
                    text: "Select Camera:"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomComboBox {
                    Layout.fillWidth: true
                    model: Camera.cameraNames
                    currentIndex: Camera.currentCameraIndex

                    onActivated: (index) => {
                        Camera.switchCam(index)
                    }
                }
            }

            // CAM QUALITY
            ColumnLayout {
                Layout.row: 1
                Layout.fillWidth: true
                spacing: 6
                Layout.leftMargin: 20

                Text {
                    text: "Photo Quality:"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomComboBox {
                    Layout.fillWidth: true
                    model: Camera.camQualities
                    currentIndex: Camera.currentQualityIndex

                    onActivated: (index) => {
                        Camera.applyPhotoQuality(index)
                    }
                }
            }

            // VIDEO QUALITY
            ColumnLayout {
                Layout.row: 2
                Layout.fillWidth: true
                spacing: 6
                Layout.leftMargin: 20

                Text {
                    text: "Video Quality:"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomComboBox {
                    Layout.fillWidth: true
                    model: Camera.videoQualities
                    currentIndex: Camera.currentVideoQualityIndex

                    onActivated: (index) => {
                        Camera.applyVideoQuality(index)
                    }
                }
            }
        }
    }
}
