import QtQuick 2.0
import "../images"
Rectangle {
    width: 100
    height: 40
    radius: 5
    color:"transparent"

    property alias text: txt.text

    Image {
            id: back
            anchors.fill: parent
            source: "../images/img_button_normal.png"
            opacity: 0.4
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    back.source="../images/img_button_press.png"
                }
                onReleased: {
                    back.source="../images/img_button_normal.png"
                }
            }
        }
    Text {
        id: txt
        anchors.centerIn: parent
        text: qsTr("选择")
        color:"White"
    }


}
