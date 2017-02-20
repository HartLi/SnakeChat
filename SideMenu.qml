import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
Item {

    property ListModel listModel
    property string borderColor: borderColor
    property string highlightColor: highlightColor
    property alias currentIndex: listView.currentIndex

    ListView{
        id: listView
        model: listModel
        anchors.fill: parent
        highlight: Rectangle{
            color: highlightColor
            z: 10
            opacity: 0.4
        }

        delegate: Item{
            id: delegate
            width: parent.width
            height: headerHeight

            property string parentId: eParentId
            property string currentImageSource: ""

            Rectangle {
               id: rectHeader
               color: Material.color(Material.BlueGrey, Material.Shade700)
               height: headerHeight
               width: parent.width
               border.color: borderColor
               Row{
                   spacing: 10
                   height: parent.height
                   leftPadding: 10
                   topPadding: 10
                   anchors.verticalCenter: parent.verticalCenter
                   Image{
                       id: imageHeader
                       source: currentImageSource
                       anchors.verticalCenter:  parent.verticalCenter
                   }
                   Label{
                        id: lblText
                        anchors.verticalCenter:  parent.verticalCenter
                        font.pixelSize: 14
                        font.bold: true
                        color: "white"
                        font.family: "Sans-Serif"
                        text: headerText
                   }
               }
            }
            Component.onCompleted: {
               if(parentId == "0" || parentId == "-1"){
                   delegate.state = "expanded"
                   delegate.currentImageSource = expandImage
               }else{
                   delegate.state = "collapsed"
                   lblText.padding = 10
               }
            }
            onStateChanged: {

            }

            states: [
                State {
                    name: "collapsed"
                    PropertyChanges {target: delegate; visible: false}
                    PropertyChanges {target: delegate; height: 0}
                },
                State {
                    name: "expanded"
                    PropertyChanges {target: delegate; visible: true}
                    PropertyChanges {target: delegate; height: headerHeight}
                }
            ]
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(eParentId == "0"){
                        for(var i = 0; i < listModel.count; i++){
                            if(eid === listView.contentItem.children[i].parentId){
                                if(listView.contentItem.children[i].state === "expanded")
                                    listView.contentItem.children[i].state = "collapsed"
                                else
                                    listView.contentItem.children[i].state = "expanded"
                            }
                        }
                        if (delegate.currentImageSource === expandImage) {
                            delegate.currentImageSource = collapseImage;
                        }else {
                            delegate.currentImageSource = expandImage;
                        }
                    }else{
                        listView.currentIndex = index
                    }
                }
            }
        }
    }
}

