import QtQuick 1.1

Item {
    property alias image: icon.source

    Item {
        id: background

        anchors.fill: parent

        Image {
            anchors.fill: parent
            source: "../assets/noise_tile.png"
            fillMode: Image.Tile
        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.05
        }
    }

    Image {
        id: icon

        anchors.centerIn: parent
        anchors.verticalCenterOffset: -75
        fillMode: Image.PreserveAspectFit
    }
}
