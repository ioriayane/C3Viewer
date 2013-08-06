import QtQuick 2.0

GridView {
    id: root
    clip: true
    //    focus: true

    property int currentDateId: 0
    property int currentBlockId: 0
    property int currentGenreId: 0

    property string imagePath: ""

    cellWidth: 105 + 4
    cellHeight: 150 + 4

    //カーソルを設定する
    function setCurrentIndex(pos){
        if(pos > -1)
            root.currentIndex = pos
    }
    // リンク用のスタイル作成
    function getLinkStyleString(color){
        return "<style>A:link{color:'" + color + "';text-decoration:underline;}"
                + " A:visited{text-decoration:underline;}"
                + " A:hover{text-decoration:underline;}</style>";
    }

    highlightMoveDuration: 200
    highlight: Rectangle {
        width: root.cellWidth
        height: root.cellHeight
        color: "lightGray"
//        opacity: 0.2
    }

    delegate: MouseArea {
        width: image.width + 4
        height: image.height + 4
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
            anchors.centerIn: parent
            source: imagePath + model.updateId + ".PNG"
            Text {
                x: 4
                y: 4
                width: 23
                height: 23
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: model.spaceNo + model.spaceNoSub
            }
        }
    }
}
