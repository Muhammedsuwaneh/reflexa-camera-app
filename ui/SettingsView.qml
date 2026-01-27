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
        visible: root.opacity > 0

        GridLayout
        {
            anchors.topMargin: 15
            rows: 9
            rowSpacing: 20

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
                    model: CameraController.cameraNames
                    currentIndex: CameraController.currentCameraIndex

                    onActivated: (index) => {
                        CameraController.switchCam(index)
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
                    model: CameraController.camQualities
                    currentIndex: CameraController.currentQualityIndex

                    onActivated: (index) => {
                        CameraController.applyPhotoQuality(index)
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
                    model: CameraController.videoQualities
                    currentIndex: CameraController.currentVideoQualityIndex

                    onActivated: (index) => {
                        CameraController.applyVideoQuality(index)
                    }
                }
            }

            ColumnLayout {
                Layout.row: 3
                Layout.fillWidth: true
                spacing: 6
                Layout.leftMargin: 20

                Text {
                    text: "Zoom"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 2
                    Layout.fillWidth: true

                    onSliding: (value) =>
                    {
                        ImageTextureController.zoomCapture(value)
                    }
                }
            }

            ColumnLayout {
                Layout.topMargin: 15
                Layout.row: 4
                Layout.fillWidth: true
                spacing: 3
                Layout.leftMargin: 20

                Text {
                    text: "Brightness"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 2
                    Layout.fillWidth: true

                    onSliding: (value) => {
                        ImageTextureController.adjustBrightness(value)
                    }
                }
            }


            ColumnLayout {
                Layout.topMargin: 15
                Layout.row: 5
                Layout.fillWidth: true
                spacing: 3
                Layout.leftMargin: 20

                Text {
                    text: "Contrast"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 5
                    Layout.fillWidth: true

                    onSliding: (value) => {
                        ImageTextureController.adjustContrast(value)
                    }
                }
            }



            ColumnLayout {
                Layout.topMargin: 15
                Layout.row: 6
                Layout.fillWidth: true
                spacing: 3
                Layout.leftMargin: 20

                Text {
                    text: "Saturation"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 5
                    Layout.fillWidth: true
                    value: 100

                    onSliding: (value) => {
                        ImageTextureController.adjustSaturation(value)
                    }
                }
            }


            ColumnLayout {
                Layout.topMargin: 15
                Layout.row: 7
                Layout.fillWidth: true
                spacing: 3
                Layout.leftMargin: 20

                Text {
                    text: "Exposure"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 5
                    Layout.fillWidth: true

                    onSliding: (value) => {
                        console.log("sliding: " + value)
                        ImageTextureController.adjustExposure(value)
                    }
                }
            }


            ColumnLayout {
                Layout.topMargin: 15
                Layout.row: 8
                Layout.fillWidth: true
                spacing: 3
                Layout.leftMargin: 20

                Text {
                    text: "Gray Scale"
                    font.pixelSize: 15
                    color: "#ffffff"
                }

                CustomSlider
                {
                    Layout.topMargin: 5
                    Layout.fillWidth: true

                    onSliding: (value) => {
                        console.log("sliding: " + value)
                        ImageTextureController.adjustGrayScale(value)
                    }
                }
            }

        }
    }
}
