import QtQuick 2.0

ListView {
    id: root
    clip: true
    //    focus: true

    property int currentDateId: 0
    property int currentBlockId: 0
    property int currentGenreId: 0

    property string imagePath: ""

    // リンク用のスタイル作成
    function getLinkStyleString(color){
        return "<style>A:link{color:'" + color + "';text-decoration:underline;}"
                + " A:visited{text-decoration:underline;}"
                + " A:hover{text-decoration:underline;}</style>";
    }

    highlightMoveDuration: 200
    highlight: Rectangle {
        width: parent.width
        height: 160
        color: "blue"
        opacity: 0.2
    }

    delegate: MouseArea {
        width: root.width
        height: 160
        onClicked: {
            root.currentIndex = index
        }
        Component.onCompleted: {
//            root.currentDateId = model.day - 1
//            root.currentBlockId = model.blockId - 1
//            root.currentGenreId = model.genreId - 1
        }

        Image {
            id: image
            anchors.verticalCenter: parent.verticalCenter
            source: "file:///" + imagePath + model.updateId + ".PNG"
        }
        Column {
            anchors.left: image.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5
            spacing: 5
            Text { text: model.dayFull + " " + model.blockName + " " + model.spaceNo + " " + model.spaceNoSub }
            Row {
                Text { text: qsTr("Circle Name : "); anchors.verticalCenter: parent.verticalCenter }
                Column {
                    Text { text: model.circleName; font.pointSize: 14}
                    Text { text: "(" + circleKana + ")"}
                }
            }
            Text { text: qsTr("Author : ") + model.penName }
            Text { text: qsTr("Book Name : ") + model.bookName }
            Text { text: qsTr("Genre : ") + model.genreName }
            Text {
                text: qsTr("Web : ") + getLinkStyleString("blue") + "<a href='%1'>%1</a>".arg(model.url)
                elide: Text.ElideRight
                textFormat: Text.RichText
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
            }
            Text { width: parent.width; text: qsTr("Description : ") + model.description; elide: Text.ElideRight}
        }
    }
}
