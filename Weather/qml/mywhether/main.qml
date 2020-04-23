import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import "storage.js" as Storage
import QtQuick.Particles 2.0
import "images"
import "component"
Rectangle {
    id:mainWindow
    width: 320
    height: 480

    color:"black"
    ParticleSystem {
        anchors.fill: parent
        ImageParticle {
            source: "images/particle.png"
        }
        // emit particle object with a size of 20 pixels
        Emitter {
            anchors.fill: parent
            lifeSpan : 3000

            size: 5
        }
    }

    MyButton{
        id:select
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 3
        anchors.leftMargin: 5
        MouseArea{
            anchors.fill: parent
            onClicked:{
                if(mainWindow.state==="")
                {
                    mainWindow.state="select_city"
                }
            }
        }

    }
    MyButton{
        id:exit
        text:"退出"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 3
        anchors.rightMargin: 5
        MouseArea{
            anchors.fill: parent
            onClicked: Qt.quit()

        }
    }

    Flickable{
        id:flick
        width: parent.width
        anchors.top: select.bottom
        anchors.bottom: parent.bottom
        anchors.left:parent.left
        contentWidth: col.width; contentHeight: col.height
        Column{
            id:col
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter


            Text {
                id:txt1
                text: owner.city_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt
                text: "今天："
                color:"#CCCCFF"
                font.pointSize: 20
            }
            Text {
                id:txt2
                text: owner.data_y_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt3
                text: owner.week_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt4
                text: owner.temp1_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt5
                text: owner.weather1_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt6
                text: owner.wind1_data
                color:"grey"
                font.pointSize: 20
            }

            Text {
                id:txt7
                width: mainWindow.width-40
                text: owner.index_d_data
                color:"grey"
                font.pointSize: 20
                wrapMode: Text.WordWrap
            }
            Text {
                id:txt8
                text: "明天："
                color:"#CCCCFF"
                font.pointSize: 20
            }
            Text {
                id:txt9
                text: owner.weather2_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt10
                text: owner.temp2_data
                color:"grey"
                font.pointSize: 20
            }
            Text {
                id:txt11
                text: owner.wind2_data
                color:"grey"
                font.pointSize: 20
            }
        }

    }

    Setting{
        id:settings
        anchors.top:select.bottom
        anchors.topMargin: 10
        anchors.left: select.left
        anchors.right: exit.right
    }


    states: [
        State {
            name: "select_city"
            PropertyChanges {
                target: settings
                opacity:0.9
            }
            PropertyChanges {
                target: settings
                rotation:360

            }
        }
    ]
    Component.onCompleted: {
        // 初始化数据库
        Storage.initialize();

        if(Storage.judge()==="false")
        {
            Storage.setSetting("default","101010100");
            console.log("重设")
        }
        owner.achivedata(Storage.getSetting("default"));
    }
}
