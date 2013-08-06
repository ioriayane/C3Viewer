import QtQuick 2.0
import QtQuick.Controls 1.0
import jp.relog.comikedatamodel 1.0

Rectangle {
    id: root
    color: "#aaaaaaaa"

    property string dbFilePath: ""
    property alias model: tableView.model


    Row {
        id: control
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5

        //検索キーワード入力
        TextField {
            id: searchKeyword
            font.pointSize: 10
            onAccepted: startSearch(searchKeyword.text)
        }

        Button {
            id: searchButton
            text: qsTr("Search")
            onClicked: {
                model = 0
                model = comike
                model.queryKeyword(searchKeyword.text)
            }
        }
        Button {
            onClicked: model = 0
        }
    }


    TableView {
        id: tableView
        anchors.top: control.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10

        //列とモデルを結びつける
        TableViewColumn { role: "spaceNo"; title: "No"; }
        TableViewColumn { role: "circleName"; title: "Name"; }
        //モデル
//        model: ComikeDataModel {
//            id: comike
//            dbFilePath: root.dbFilePath
//        }
    }
}
