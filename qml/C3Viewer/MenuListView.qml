import QtQuick 2.0

ListView {
    clip: true

    highlightMoveDuration: 200
    highlight: Rectangle {
        color: "blue"
        opacity: 0.2
    }
}
