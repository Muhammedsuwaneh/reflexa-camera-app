import QtQuick

Item {
    id: root
    clip: true

    signal returnClicked()

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

            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCubic
                }
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
        anchors.leftMargin: 20
        anchors.bottomMargin: 15
        width: 40
        height: 35
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
        height: 50
        radius: 20
        color: "#F9FAFB"
        opacity: .5

        visible: recentPhoto.opacity > 0

        Behavior on opacity {
            NumberAnimation { duration: 180 }
        }
    }
}
