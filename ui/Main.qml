import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 1280
    height: 900
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Window
    color: "transparent"

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
                    target: camera

                    function onFrameCleared() {
                        cameraView.source = ""
                    }

                    function onFrameChanged() {
                        cameraView.source = "image://camera/live?" + Date.now()
                    }
                }
            }
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

        Button
        {
            id: filtersButton
            width: 150
            height: 28
            flat: true
            z: 30

            anchors.right: parent.right
            anchors.bottom: controlsView.top
            anchors.rightMargin: 30
            anchors.bottomMargin: 12

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

        ControlsView {
            id: controlsView
            Layout.row: 2
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            z: 20
        }
    }
}
