import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    id: root
    height: text.height * 3
    gradient: Gradient {
        GradientStop { position: 0 ; color: "#ccc" }
        GradientStop { position: 1 ; color: "#aaa"}
    }
    property alias text: text.text
    signal doSearch(string keyword)

    //タイトルとか
    Row {
        anchors.fill: parent
        anchors.leftMargin: 5
        spacing: 5
        Image {
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            source: "images/menu.png"
        }
        Text{
            id: text
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 10
        }
    }

    //検索キーワード入力
    TextField {
        id: searchKeyword
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
        font.pointSize: 10
        width: 200
        //Enterで検索
        onAccepted: doSearch(searchKeyword.text)
        //ESCで検索クリア
        Keys.onEscapePressed: searchKeyword.clearSearch()
        //取り消しボタン
        Image {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 5
            width: height
            height: parent.font.pointSize
            visible: parent.length > 0
            source: "images/clear_mark.png"
            MouseArea {
                anchors.fill: parent
                onClicked: searchKeyword.clearSearch()
            }
        }
        //検索クリア
        function clearSearch(){
            searchKeyword.text = ""
            root.doSearch("")
        }
    }
}
