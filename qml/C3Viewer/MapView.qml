import QtQuick 2.0
import QtQuick.Controls 1.0

ScrollView {

//    color: "white"

    property alias source: image.source
    property point cursor: "-1,-1"

    Image {
        id: image
//        height: parent.height
        fillMode: Image.PreserveAspectFit

    }
}
