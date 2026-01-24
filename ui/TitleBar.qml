import QtQuick
import QtQuick.Controls

Item {
    id: root
    clip: true
    width: parent.width
    height: parent.height

    Rectangle {
        anchors.fill: parent
        radius: 12
        color: "#0b1a2a"
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 12
        color: "#0b1a2a"
    }

    Row {
        spacing: 10
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter

        Item {
            width: 20
            height: 40

            Image {
                anchors.centerIn: parent
                width: 20
                height: 40
                mipmap: true
                fillMode: Image.PreserveAspectFit
                source: "../assets/logo.png"
            }
        }

        Row {
            anchors.verticalCenter: parent.verticalCenter

            Text {
                text: "REFLE"
                color: "white"
                font.pixelSize: 15
                font.bold: true
            }
            Text {
                text: "XA"
                color: "#50DD60"
                font.pixelSize: 15
                font.bold: true
            }
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: controlButtons.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        onPressed: WindowController.startDrag(mouse.x, mouse.y)
        onDoubleClicked: {
            WindowController.maximize()
        }
    }

    Row {
        id: controlButtons
        spacing: 15
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter

        ControlButton { backgroundColor: "#FFD230"; onClicked: WindowController.minimize() }
        ControlButton { backgroundColor: "#05DF72"; onClicked: WindowController.maximize() }
        ControlButton {
            backgroundColor: "#F4320B"
            onClicked: WindowController.close()
        }
    }
}
