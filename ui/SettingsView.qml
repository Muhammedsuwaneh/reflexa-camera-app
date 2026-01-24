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
                    model: camera.cameraNames
                    currentIndex: camera.currentCameraIndex

                    onActivated: (index) => {
                        camera.switchCam(index)
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
                    model: camera.camQualities
                    currentIndex: camera.currentQualityIndex

                    onActivated: (index) => {
                        camera.applyPhotoQuality(index)
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
                    model: camera.videoQualities
                    currentIndex: camera.currentVideoQualityIndex

                    onActivated: (index) => {
                        camera.applyVideoQuality(index)
                    }
                }
            }
        }
    }
}
