import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    clip: true

    signal returnClicked()

    property string mediaType: Camera.currentMediaType

    property real zoomFactor: 1.0
    property real minZoom: 0.6
    property real maxZoom: 3.0

    property real rotationAngle: 0


    Rectangle {
        id: roundedBackground
        anchors.fill: parent
        radius: 18
        color: "#000000"

        Image {
            id: recentPhoto
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: 10
            }

            fillMode: Image.PreserveAspectFit
            smooth: true
            cache: false
            opacity: 0

            transform: [
                Scale {
                    origin.x: recentPhoto.width / 2
                    origin.y: recentPhoto.height / 2
                    xScale: zoomFactor
                    yScale: zoomFactor
                },
                Rotation {
                    origin.x: recentPhoto.width / 2
                    origin.y: recentPhoto.height / 2
                    angle: rotationAngle
                }
            ]

            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }

            Behavior on scale {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }

            Behavior on rotation {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }

            Connections {
                target: Camera
                function onRecentCapturedChanged() {
                    if (Camera.recentCaptured !== null) {
                        recentPhoto.source =
                            "image://captured/current?" + Date.now()
                        recentPhoto.opacity = 1
                    }
                }
            }
        }
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 25
        anchors.bottomMargin: 25
        width: 50
        height: 50
        source: "../assets/camera.png"
        fillMode: Image.PreserveAspectFit
        smooth: true

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: root.returnClicked()
        }
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 40
        height: 40
        source: "../assets/arrow_left.png"
        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: recentPhoto.opacity !== 0

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: console.log("Previous media")
        }
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        width: 40
        height: 40
        source: "../assets/arrow_right.png"
        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: recentPhoto.opacity !== 0

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: console.log("Next media")
        }
    }

    Text {
        anchors.centerIn: parent
        text: "No media yet"
        color: "#888"
        font.pixelSize: 18
        visible: recentPhoto.opacity === 0
    }

    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 20
        color: "#000000"
    }


    Rectangle {
        id: galleryControls
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50

        width: 300
        height: 60
        radius: 20
        color: "#F9FAFB"
        opacity: 0.85

        visible: recentPhoto.opacity > 0

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 30
            layoutDirection: Qt.LeftToRight

            Item { Layout.fillWidth: true } // spacer (left)

            Image {
                width: 22
                height: 22
                source: "../assets/delete.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        console.log("Deleting from list and PC")
                    }
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/rotate.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter
                visible: mediaType == "photo" ? true : false

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        rotationAngle = (rotationAngle + 90) % 360
                    }
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/zoom_in.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter
                visible: mediaType == "photo" ? true : false

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        zoomFactor = Math.min(maxZoom, zoomFactor + 0.2)
                    }
                }
            }


            Image {
                width: 22
                height: 22
                source: "../assets/zoom_out.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter
                visible: mediaType == "photo" ? true : false

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        zoomFactor = Math.max(minZoom, zoomFactor - 0.2)
                    }
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/play.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter
                visible: mediaType == "photo" ? false : true

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        console.log("Play")
                    }
                }
            }

            Image {
                width: 22
                height: 22
                source: "../assets/pause.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
                Layout.alignment: Qt.AlignVCenter
                visible: mediaType == "photo" ? false : true

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        console.log("Pause")
                    }
                }
            }

            Item { Layout.fillWidth: true } // spacer (right)
        }

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }
    }
}
