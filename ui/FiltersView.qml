import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {

    id: root
    clip: true
    color: "transparent"
    width: parent.width

    property string oldFilter: "";

    Rectangle
    {
        anchors.fill: parent
        color: "#111111"

        ColumnLayout
        {
            GridLayout
            {
                Layout.topMargin: 50
                Layout.leftMargin: 50
                rows: 2
                columns: 2
                rowSpacing: 15
                columnSpacing: 15

                Repeater {
                    model: [
                        "Gray Scale",
                        "Negative / Invert",
                        "High Contrast",
                        "Gaussian Blur",
                        "Skin Smoothing",
                        "Sepia (Warm)",
                        "No filter"
                    ]

                    ColumnLayout {
                        spacing: 6
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            text: modelData
                            font.pixelSize: 12
                            color: "#ffffff"
                            horizontalAlignment: Text.AlignLeft
                            Layout.alignment: Qt.AlignLeft
                        }

                        Image {
                            source: "../assets/filter_images/1.jpg"
                            opacity: buttonMouseArea.containsMouse ? 0.8 : 1

                            Layout.preferredHeight: 70
                            Layout.preferredWidth: 100
                            Layout.alignment: Qt.AlignHCenter

                            MouseArea {
                                id: buttonMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor

                                onClicked: {
                                    if (root.oldFilter !== modelData) {
                                        ImageFiltersController.applyFilter(modelData)
                                        root.oldFilter = modelData
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
