import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.0

Window {
    id: root
    width: content.width * 1.5
    height: content.height * 1.5

    title: qsTr("About")
    color: "lightGray"

    Column {
        id: content
        anchors.centerIn: parent
        spacing: 10
        Text {
            text: "C3Viewer"
            font.pointSize: 20
        }
        Text {
            text: "- ComiC market dvd-rom Catalog Viewer -"
        }
        Text {
            text: "Copyright 2013 IoriAYANE"
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Close")
            onClicked: root.visible = false
        }
    }
}
