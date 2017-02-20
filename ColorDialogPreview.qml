import QtQuick 2.0
import QtQuick.Dialogs 1.2

Rectangle{
    id: rectPreview
    property alias previewColor: rectPreview.color
    property alias alphaVisible: dialogColor.showAlphaChannel
    signal colorSelected()
    signal click;
    width: 25
    height: 25
    border.color: "#33000000"
    border.width: 2
    MouseArea{
        anchors.fill: parent
        onClicked: {
            rectPreview.click()
            dialogColor.visible = true
        }
    }
    ColorDialog{
        id: dialogColor
        title: "Select color"
        color: rectPreview.color
//        showAlphaChannel: alphaVisible
        onAccepted: {
            rectPreview.color = dialogColor.color
            rectPreview.colorSelected()
        }
    }
}


