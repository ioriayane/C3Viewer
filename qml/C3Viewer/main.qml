import QtQuick 2.0
import QtQuick.Dialogs 1.0
import jp.relog.comikedatamodel 1.0
import jp.relog.operatingsystem 1.0
import QtQuick.LocalStorage 2.0
import "javascripts/Database.js" as DB

Rectangle {
    id: root
    width: 800
    height: 600

    property string catalogPath: "D:/catalog/DATA84N/"
    property string imagePath: catalogPath + "PDATA/"
    property string mapPath: catalogPath + "MDATA/"

    //起動後の処理
    Component.onCompleted: {

        //設定を読む
        if(os.type == OperatingSystem.Android){
            catalogPath = "/data/data/jp.xii.relog.c3viewer/files/../../../../sdcard/DATA84N/"
        }else if(os.type == OperatingSystem.Mac){
            catalogPath = (os.homeDirectory + "catalog/DATA84N/").replace(os.pathPrefix, "")
        }else{
            catalogPath = DB.readData("catalogPath", "")
        }
        if(catalogPath.length == 0){
            //フォルダ選択
            fileDialog.folder = os.homeDirectory
            fileDialog.open()
        }else{
            //データベース更新
            comike.update(true)
        }

        //メインメニュー作成
        menuModel.append({"id": "0", "name": qsTr("Date&Place"), "type": "menu"})
        menuModel.append({"id": "1", "name": qsTr("Ganre"), "type": "menu"})
        menuModel.append({"id": "2", "name": qsTr("Map"), "type": "menu"})
        menuModel.append({"id": "3", "name": qsTr("Search"), "type": "menu"})
        menuModel.append({"id": "99", "name": qsTr("About"), "type": "menu"})
    }
    //終了処理
    Component.onDestruction: {
        //設定保存
        DB.saveData("catalogPath", catalogPath)
    }

    //OS情報
    OperatingSystem {
        id: os
    }
    //ファイル選択
    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a comeket catalog directory")
        modality: Qt.ApplicationModal
        selectFolder: true
        onAccepted: {
            //パスを設定する
            catalogPath = (fileDialog.fileUrls + "/").replace(os.pathPrefix, "")
            console.debug(catalogPath)
            //データベース更新
            comike.update(true)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    //カタログのモデル
    ComikeDataModel {
        id: comike
        dbFilePath: catalogPath + "catalog.db"
        queryDayId: daysList.currentDay
        queryBlockId: ""
        queryGenreId: ""

        property int prevDayPos: 0  //前回選択した日付の最初のIDを保存しておく（場所位置を調べるときの開始位置）

        //サークル情報登録完了
        onCirclesChanged: {
        }

        //メニュー（日付一覧）をDBの情報から作る
        onDaysChanged: {
            var items;
            daysModel.clear()
            for(var i=0; i<days.length; i++){
                items = days[i].split("\t")
                daysModel.append({"id": items[0], "name": items[1], "type": "days"})
            }
        }
        //メニュー（ブロック一覧）をDBの情報から作る
        onBlocksChanged: {
            var items;
            blocksModel.clear()
            for(var i=0; i<blocks.length; i++){
                items = blocks[i].split("\t")
                blocksModel.append({"id": items[0], "name": items[1], "type": "blocks"})
            }
        }
        //メニュー（ジャンル一覧）をDBの情報から作る
        onGenresChanged: {
            var items;
            genresModel.clear()
            for(var i=0; i<genres.length; i++){
                items = genres[i].split("\t")
                genresModel.append({"id": items[0], "name": items[1], "type": "genres"})
            }
        }
    }


    //aboutダイアログ
    AboutDialog {
        id: aboutWindow
        visible: false
    }

    //実際のコンテンツ
    ActionBar {
        id: actionBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        onDoSearch: comike.queryKeyword = keyword
    }
    Item {
        id: splitView
        anchors.top: actionBar.bottom
        anchors.bottom: parent.bottom
        x: 0
        width: parent.width

        //メニュー領域
        Rectangle {
            id: menuArea
            anchors.top: parent.top
            anchors.left: parent.left
//            anchors.bottom: parent.bottom
            width: 300
            height: parent.height * 0.5
            color: "lightgray"
            //メインメニュー
            ListView {
                id: menu
                width: 100
                height: parent.height
                clip: true
                model: ListModel { id: menuModel }
                delegate: MenuButton {
                    width: parent.width
                    text: model.name
                    onClicked: {
                        if(model.id === "99")
                            aboutWindow.visible = true
                        else
                            menu.currentMenuId = model.id
                    }
                }
                //状態管理用に現在選択中の項目
                property string currentMenuId: "0"

            }
            //日付と場所
            Row {
                id: dateAndPalace
                anchors.left: menu.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                //日付一覧
                MenuListView {
                    id: daysList
                    width: 100
                    height: parent.height
                    model: ListModel { id: daysModel }
                    delegate: MenuButton {
                        width: parent.width
                        text: model.name
                        onClicked: {
                            //現在の日付ID
                            daysList.currentDay = model.id
                            //リストのカーソルを移動する
                            daysList.currentIndex = index
                        }
                    }
                    //現在の日付ID
                    property string currentDay: "1"
                }
                //ブロック一覧
                MenuListView {
                    id: blocksList
                    width: 100
                    height: parent.height
                    model: ListModel { id: blocksModel }
                    delegate: MenuButton {
                        width: parent.width
                        text: model.name
                        onClicked: {
                            //リストのカーソルを移動する
                            blocksList.currentIndex = index
                            //ブロックの頭のインデックスを調べるてハイライトを移動する
                            list.setCurrentIndex(comike.getBlockStartIndex(model.id, comike.prevDayPos))
                        }
                    }
                }
            }
            //ジャンル一覧
            MenuListView {
                id: genreList
                anchors.fill: dateAndPalace
                width: 200
                height: parent.height
                model: ListModel { id: genresModel }
                delegate: MenuButton {
                    width: parent.width
                    text: model.name
                    onClicked: {
                        //リストのカーソルを移動する
                        genreList.currentIndex = index
                        //ジャンルの頭のインデックスを調べるてハイライト移動
                        list.setCurrentIndex(comike.getGenreStartIndex(model.id))
                    }
                }
                visible: false
            }
        }
//        //マップ
//        MapView {
//            id: map
//            anchors.left: menuArea.left
//            anchors.right: menuArea.right
//            anchors.top: menuArea.bottom
//            anchors.bottom: parent.bottom

//            source: "%1%2MAP%3%4.PNG".arg(os.pathPrefix).arg(root.mapPath).arg(daysList.currentDay).arg("E456")
////            source: "%1%2MAP%3%4.PNG".arg("file:///").arg(root.mapPath).arg(daysList.currentDay).arg("E456")
//        }
        //サークル情報
        CircleGridView {
            id: list
            anchors.left: menuArea.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 5
            focus: true

            model: comike
            imagePath: "%1%2".arg(os.pathPrefix).arg(root.imagePath)
        }


        //検索ウインドウ
        SearchView {
            id: searchView
            anchors.fill: parent
            visible: false
            model: 0
        }

    }

    //モデルとリストとの結びつけ管理
    StateGroup {
        states: [
            //日付と場所
            State {
                when: menu.currentMenuId == "0"
                PropertyChanges { target: actionBar; text: qsTr("Date&Place") }
                PropertyChanges { target: dateAndPalace; visible: true }
                PropertyChanges { target: genreList; visible: false }
            }
            //ジャンル
            , State {
                when: menu.currentMenuId == "1"
                PropertyChanges { target: actionBar; text: qsTr("Ganre") }
                PropertyChanges { target: dateAndPalace; visible: false }
                PropertyChanges { target: genreList; visible: true }
            }
            //マップ
            , State {
                when: menu.currentMenuId == "2"
                PropertyChanges { target: actionBar; text: qsTr("Map") }

            }
            //検索
            , State {
                when: menu.currentMenuId == "3"
                PropertyChanges { target: actionBar; text: qsTr("Search") }
                PropertyChanges { target: searchView; visible: true; model: comike}
                PropertyChanges { target: list; model: 0 }

            }
        ]
    }

}
