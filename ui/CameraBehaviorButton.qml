import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 60
    height: 60

    property url iconSource: ""
    property bool isActive: false

    signal clicked()

    Rectangle {
        id: bg
        anchors.centerIn: parent
        width: 40
        height: 40
        radius: width / 2
        border.color: "#fff"
        border.width: 2
        antialiasing: true
        color: "transparent"

        Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            radius: width / 2
            color: root.isActive ? "#DDC050" : "transparent"
        }

        Image {
            anchors.centerIn: parent
            width: 20
            height: 20
            source: root.iconSource
            fillMode: Image.PreserveAspectFit
            mipmap: true
            smooth: true
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
