import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    implicitWidth: 220
    implicitHeight: 45

    property var model: []
    property int currentIndex: 0
    property string currentText: model.length > 0 ? model[currentIndex] : ""

    signal activated(int index)

    Rectangle {
        id: background
        anchors.fill: parent
        radius: 10
        color: "#192838"
        border.color: "#2c3e55"
        border.width: 1
    }

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent
        anchors.leftMargin: 13

        Text {
            id: selectedText
            text: root.currentText
            color: "#ffffff"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            font.pixelSize: 14
        }

        Image {
            source: "../assets/dropdown.png"
            width: 7
            height: 7
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignVCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: popup.open()
    }

    Popup {
        id: popup
        x: 0
        y: root.height + 6
        width: root.width
        padding: 6
        modal: false
        focus: true

        background: Rectangle {
            radius: 10
            color: "#0f2238"
            border.color: "#2c3e55"
            border.width: 1
        }

        ListView {
            width: parent.width
            implicitHeight: Math.min(contentHeight, 200)
            model: root.model
            clip: true

            delegate: Rectangle {
                width: parent.width
                height: 36
                color: index === root.currentIndex ? "#1e3a5f" : "transparent"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    text: modelData
                    color: "#ffffff"
                    font.pixelSize: 13
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.currentIndex = index
                        root.activated(index)
                        popup.close()
                    }
                }
            }
        }
    }
}
