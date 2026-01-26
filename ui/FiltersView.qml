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
                columns: 2
                rowSpacing: 15
                columnSpacing: 15

                Repeater
                {
                    model: ["Gray Scale", "Negative / Invert", "High Contrast", "Gaussian Blur", "Skin Smoothing", "Sepia (Warm)"]

                    Rectangle
                    {
                        radius: 8
                        color: "#e5e7eb"
                        border.color: "#cad5e2"
                        border.width: 1
                        opacity:  buttonMouseArea.containsMouse ? .8 : 1

                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 120

                        Text {
                           anchors.centerIn: parent
                           text: modelData
                           font.pixelSize: 12
                           color: "#111827"
                        }

                        MouseArea
                        {
                            id: buttonMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if(root.oldFilter != modelData)
                                {
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
