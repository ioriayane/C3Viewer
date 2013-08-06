import QtQuick 2.0
//import QtQuick.Controls 1.0
//import QtQuick.Controls.Styles 1.0

MouseArea {
    id: root
    height: text.height * 2.5
    hoverEnabled: true
    property alias text: text.text
    Row {
        anchors.fill: parent
        spacing: 5
        Image {
            anchors.verticalCenter: parent.verticalCenter
            height: text.height
            fillMode: Image.PreserveAspectFit
            source: "images/arrow01.png"
        }
        Text{
            id: text
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 10
        }
    }
    Rectangle {
        id: filter
        anchors.fill: parent
        color: "blue"
        opacity: 0
    }
    states: [
        State {
            when: pressed
            PropertyChanges {
                target: filter
                color: "red"
                opacity: 0.2
            }
        }
        , State {
            when: containsMouse
            PropertyChanges {
                target: filter
                color: "orange"
                opacity: 0.2
            }
        }

    ]

//    Button {
//        id: button
//        width: text.paintedWidth
//        style: ButtonStyle {
//            background: Rectangle {
//                implicitWidth: 100
//                implicitHeight: 25

//                border.width: control.activeFocus ? 2 : 1
//                border.color: "#888"
//                radius: 4
//                gradient: Gradient {
//                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
//                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
//                }
//            }
//        }
//        onClicked: root.clicked(mouse)
//    }
}
