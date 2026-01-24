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
        width: root.isActive ? 60 : 40
        height: root.isActive ? 60 : 40
        radius: width / 2
        border.color: "#fff"
        border.width: root.isActive ? 5 : 2
        antialiasing: true
        color: "transparent"

        Rectangle {
            anchors.centerIn: parent
            width: parent.width - 6
            height: parent.height - 6
            radius: width / 2
            color: root.isActive ? "#DD5450" : "transparent"
        }

        Image {
            anchors.centerIn: parent
            width: root.isActive ? 30 : 20
            height: root.isActive ? 30 : 20
            source: root.iconSource
            fillMode: Image.PreserveAspectFit
            mipmap: true
            smooth: true
        }

        Behavior on width { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        Behavior on height { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
