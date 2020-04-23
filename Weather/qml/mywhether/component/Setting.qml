import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import "../storage.js" as Storage
Rectangle{
    id:settings
    width: 300
    height: 200
    radius: 15
    color:"grey"
    opacity: 0
    MyInput{
        id:input
        anchors.topMargin: 10
        anchors.leftMargin: 10

        anchors.left:settings.left
        anchors.right: btn.left

        text:"广州"
    }

    MyButton{
        id:btn
        width: 60
        height: 40
        anchors.right: settings.right
        text:"确认"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                mainWindow.state=""
                var res=owner.findCityID(input.text);

                //console.log(res)
                if(Storage.setSetting("default",res)==="OK")
                  {
                      owner.achivedata(res)
                      console.log(res)
                  }
            }
        }
    }
    Behavior on opacity{
        NumberAnimation{
            duration: 300
        }
    }
    Behavior on rotation {
        NumberAnimation{
            duration: 300
        }
    }

}
