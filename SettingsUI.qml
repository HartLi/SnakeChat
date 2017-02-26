import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
Window {
    id: settingsUI
    height: 500
    width: 600
    x: 100
    y: 100
    title: qsTr("Snake Settings")
    flags: Qt.WindowSystemMenuHint | Qt.WindowTitleHint | Qt.WindowStaysOnTopHint |  Qt.Window | Qt.WindowCloseButtonHint
    signal settingsClosed

    Material.background: Material.color(Material.BlueGrey, Material.Shade900)
    Material.theme: Material.Dark
    MessageDialog {
        id: messageDialog
        title: "Error"
        onAccepted: {
            messageDialog.close()
        }
    }
    Rectangle{
        id: sideMenu
        width: 0.30 * settingsUI.width
        height: settingsUI.height
        color: Material.color(Material.BlueGrey, Material.Shade800)


        SideMenu{
            id: listSideMenu
            listModel: modelMenu
            borderColor: Material.color(Material.BlueGrey, Material.Shade900)
            highlightColor: Material.color(Material.DeepPurple)
            width: settingsUI.width * 0.33
            height: settingsUI.height
        }

        ListModel
        {
            id: modelMenu
            ListElement {
                headerText: "General"
                eid: "pageGeneral"
                headerHeight: 30
                eParentId: "-1"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Appearance"
                eid: "pageAppear"
                headerHeight: 30
                eParentId: "0"
                collapseImage: "icons/arrow-203-16.png"
                expandImage: "icons/arrow-24-16.png"
            }
            ListElement {
                headerText: "Window"
                eid: "pageWindowAppear"
                headerHeight: 30
                eParentId: "pageAppear"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Message"
                eid: "pageMsgAppear"
                headerHeight: 30
                eParentId: "pageAppear"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Chat"
                eid: "pageChat"
                headerHeight: 30
                eParentId: "-1"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Services"
                eid: "pageServices"
                headerHeight: 30
                eParentId: "0"
                collapseImage: "icons/arrow-203-16.png"
                expandImage: "icons/arrow-24-16.png"
            }
            ListElement {
                headerText: "Authorization"
                eid: "auth"
                headerHeight: 30
                eParentId: "pageServices"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Connection"
                eid: "connection"
                headerHeight: 30
                eParentId: "pageServices"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "Webserver"
                eid: "webServer"
                headerHeight: 30
                eParentId: "-1"
                collapseImage: ""
                expandImage: ""
            }
            ListElement {
                headerText: "About"
                eid: "pageAbout"
                headerHeight: 30
                eParentId: "-1"
                collapseImage: ""
                expandImage: ""
            }
        }
    }
    StackLayout{
        id: stackLayout
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: sideMenu.right
        currentIndex: listSideMenu.currentIndex

        Page{
            id: pageGeneral
            title: qsTr("General")
            visible: true
            padding: 15
            font.family: "Sans-Serif"
            header: Label{
                id: labelTitleGeneral
                font.pixelSize: 16
                text: pageGeneral.title
                height: 40
                padding: 10
            }
            Column{
                spacing: 10
                Switch{
                    id: topMost
                    text: "Stay on top"
                    onClicked: {
                        if(topMost.checked)
                            mainWindow.flags += Qt.WindowStaysOnTopHint
                        else{
                            mainWindow.flags -= Qt.WindowStaysOnTopHint

                        }
                        settings.setValue("topmost", checked)
                    }
                    function getSettingsChecked(){
                        if(settings.value("topmost", false) === "true")
                            return true
                        else
                            return false
                    }
                    Component.onCompleted: {
                        checked = getSettingsChecked()
                    }

                }
                Switch{
                    id: windowTittleBar
                    text: "Native window titlebar"
                    onClicked: {
                        if(checked){
                            mainWindow.flags -= Qt.FramelessWindowHint
                            mainWindow.flags += Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint
                        }else{
                            mainWindow.flags += Qt.FramelessWindowHint
                            mainWindow.flags -= Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint
                        }
                        settings.setValue("native_titlebar", checked)
                    }
                    Component.onCompleted: {
                        checked = getSettingsChecked()
                    }
                    function getSettingsChecked(){
                        if(settings.value("native_titlebar", false) === "true")
                            return true
                        else
                            return false
                    }
                }
                Row{
                    spacing: 10
                    Label{
                        text: "Messages socket port:"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    TextField{
                        id: spinBoxPort
                        height: 25
                        width: 100
                        topPadding: 25
                        verticalAlignment: TextField.AlignVCenter
                        horizontalAlignment: TextField.AlignHCenter
                        inputMask: "D9990"
                        background: Rectangle{
                            anchors.fill: parent
                            color: Material.color(Material.BlueGrey, Material.Shade700)
                        }
                        cursorPosition: text.length

                        text: messagesPort
                        onTextChanged: {
                            settings.writeMessagesPort(text)
                        }
                    }
                }
            }
        }
        Item{

        }

        Page{
            id: pageWindow
            title: qsTr("Window Appearance")
            font.family: "Sans-Serif"
            font.pixelSize: 16
            padding: 15
            header: Label{
                font.pixelSize: 16
                text: pageWindow.title
                height: 40
                padding: 10
            }

            Column{
                spacing: 10
                Row{
                    spacing: 5
                    ColorDialogPreview{
                        id: clrBack
                        previewColor: mainWindow.color
                        anchors.verticalCenter: parent.verticalCenter
                        onColorSelected: {
                            mainWindow.color = clrBack.previewColor
                            settings.setValue("background_color", clrBack.previewColor, "WindowAppearance")
                        }
                        onClick: {
                            clrBack.alphaVisible = !windowTittleBar.checked
                        }

                    }
                    Label{
                        text: "Background color"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Label{
                    text: "Window opacity"
                }

                Slider{
                    id: slideWindowOpacity
                    from: 0.2
                    to: 1
                    stepSize: 0.0025
                    value: settings.value("window_opacity", 1, "WindowAppearance")
                    onValueChanged: {
                        mainWindow.opacity = slideWindowOpacity.value
                        settings.setValue("window_opacity", slideWindowOpacity.value, "WindowAppearance")
                    }
                }

            }
        }
        Page{
            id: pageMessage
            title: qsTr("Message Appearance")
            font.family: "Sans-Serif"
            font.pixelSize: 16
            padding: 15
            header: Label{
                font.pixelSize: 16
                text: pageMessage.title
                height: 40
                padding: 10
            }
            Column{
                spacing: 10
                Row{
                    spacing: 5
                    ColorDialogPreview{
                        previewColor: settings.value("background_color", "black", "Message")
                        alphaVisible: true
                        anchors.verticalCenter: parent.verticalCenter
                        onColorSelected: {
                            settings.setValue("background_color", previewColor, "Message")
                        }
                    }
                    Label{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Background color"
                    }

                }
                Row{
                    spacing: 5
                    ColorDialogPreview{
                        anchors.verticalCenter: parent.verticalCenter
                        previewColor: settings.value("nickname_color", "purple", "Message")
                        onColorSelected: {
                            settings.setValue("nickname_color", previewColor, "Message")
                        }
                    }
                    Label{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Nickname color"
                    }

                }
                Row{
                    spacing: 5
                    ColorDialogPreview{
                        anchors.verticalCenter: parent.verticalCenter
                        previewColor: settings.value("text_color", "white", "Message")
                        onColorSelected: {
                            settings.setValue("text_color", previewColor, "Message")
                        }
                    }
                    Label{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Text color"
                    }

                }
                Row{
                    spacing: 5
                    ColorDialogPreview{
                        anchors.verticalCenter: parent.verticalCenter
                        previewColor: settings.value("highlight_color", Material.color(Material.Orange), "Message")
                        onColorSelected: {
                            settings.setValue("highlight_color", previewColor, "Message")
                        }
                    }
                    Label{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Highlight text color"
                    }

                }
                Switch{
                    checked: settings.value("twitch_colors", "true", "Message") === "true" ? true : false
                    text: "Twitch nickname colors"
                    onClicked: {
                        settings.setValue("twitch_colors", checked, "Message")
                    }
                }
                Row{
                    spacing: 10
                    height: 20
                    Label{
                        text: "Message animation"
                        verticalAlignment: Label.AlignVCenter
                    }
                    ComboBox{
                        id: cmbAnimation
                        width: 150
                        model: ListModel {
                            ListElement { text: "bounceIn"}
                            ListElement { text: "bounceInDown"}
                            ListElement { text: "bounceInLeft"}
                            ListElement { text: "bounceInRight"}
                            ListElement { text: "bounceInUp"}
                            ListElement { text: "fadeIn"}
                            ListElement { text: "fadeInDown"}
                            ListElement { text: "fadeInDownBig"}
                            ListElement { text: "fadeInLeft"}
                            ListElement { text: "fadeInLeftBig"}
                            ListElement { text: "fadeInRight"}
                            ListElement { text: "fadeInRightBig"}
                            ListElement { text: "fadeInUp"}
                            ListElement { text: "fadeInUpBig"}
                            ListElement { text: "flipInX"}
                            ListElement { text: "flipInY"}
                            ListElement { text: "lightSpeedIn"}
                            ListElement { text: "rotateIn"}
                            ListElement { text: "rotateInDownLeft"}
                            ListElement { text: "rotateInDownRight"}
                            ListElement { text: "rotateInUpLeft"}
                            ListElement { text: "rotateInUpRight"}
                            ListElement { text: "rollIn"}
                            ListElement { text: "slideInDown"}
                            ListElement { text: "slideInLeft"}
                            ListElement { text: "slideInRight"}
                            ListElement { text: "slideInUp"}
                            ListElement { text: "zoomIn"}
                            ListElement { text: "zoomInDown"}
                            ListElement { text: "zoomInLeft"}
                            ListElement { text: "zoomInRight"}
                            ListElement { text: "zoomInUp"}
                        }
                        anchors.verticalCenter: parent.verticalCenter
                        onActivated: {
                            settings.setValue("animation", model.get(currentIndex).text, "Message")
                        }
                        Component.onCompleted: {
                            currentIndex = find(settings.value("animation", "slideUp", "Message"))
                        }

                    }
                }


            }

        }
        Page{
            id: pageChat
            title: qsTr("Chat")
            font.family: "Sans-Serif"
            padding: 15
            header: Label{
                font.pixelSize: 16
                text: pageChat.title
                height: 40
                padding: 10
            }
            Column{
                Switch{
                    text: "Show images"
                    checked: settings.value("show_images", true, "Chat")
                    onClicked: {
                        settings.setValue("show_images", checked, "Chat")
                    }
                }
                Switch{
                    text: "Show message time"
                    checked: settings.value("show_time", true, "Chat")
                    onClicked: {
                        settings.setValue("show_time", checked, "Chat")
                    }
                }
                Label{
                    text: "Message delay: "+ sliderDelay.value.toFixed() +" ms"
                }
                Slider{
                    id: sliderDelay

                    anchors.topMargin: 30
                    stepSize: 25.0
                    from: 50
                    to: 10000
                    value: settings.value("message_delay", 100, "Chat")
                    snapMode: "SnapAlways"
                    onValueChanged: {
                        settings.setValue("message_delay", sliderDelay.value.toFixed(), "Chat")
                    }
                }


            }

        }
        Page{
            id: pageServices
            title: qsTr("Services")
        }
        Page{
            id: pageAuth
            title: qsTr("Authorizations")
            font.family: "Sans-Serif"
            font.pixelSize: 16
            padding: 15
            header: Label{
                id: lblAuth
                font.pixelSize: 16
                text: pageAuth.title
                height: 40
                padding: 10

            }
            Button{
                id: authTwitchHeader
                height: 40
                width: parent.width
                contentItem:Item{
                    anchors.fill: parent
                    Image{
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: lblTwitchAuth.left
                        source: "/web/images/twitch.png"
                    }
                    Label{
                        id: lblTwitchAuth
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Twitch"
                        font.pixelSize: 20
                    }
                }

                background: LinearGradient{
                    start: Qt.point(0, 0)
                    end: Qt.point(0, 40)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#33ffffff"  }
                        GradientStop { position: 1.0; color: mainWindow.color}

                    }
                }
                onClicked: {
                    if(authTwitchBody.state === "expanded")
                        authTwitchBody.state = "collapsed"
                    else
                        authTwitchBody.state = "expanded"
                }
            }
            Rectangle{
                id: authTwitchBody
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: authTwitchHeader.bottom
                height: authTwitchBody.childrenRect.height
                color: Material.color(Material.BlueGrey, Material.Shade800)
                Column{
                    spacing: 10
                    padding: 10
                    anchors.fill: parent
                    Button{
                        text: "Authorize"

                        font.capitalization: Font.Capitalize
                        flat: true
                        height: 35
                        anchors.horizontalCenter: parent.horizontalCenter
                        background: Rectangle{
                            color: "#6441A4"
                        }
                        onClicked: {
                            Qt.openUrlExternally("https://api.twitch.tv/kraken/oauth2/authorize?response_type=token&client_id=pcjraf3wf7s8oyeea2l6obepbs9xnh&redirect_uri=http%3A%2F%2Flocalhost%3A8080%2Ftransmit_hash.html&scope=chat_login+user_read+channel_read+user_blocks_read+user_blocks_edit+channel_subscriptions+channel_feed_read+user_subscriptions+channel_stream&force_verify=true
    ")
                        }
                    }
                    Label{
                        id: lblAuthUser
                        anchors.horizontalCenter: parent.horizontalCenter
                        Component.onCompleted: {
                            secSettings.beginGroup("Twitch")
                            var username = secSettings.value("twitch_username")
                            secSettings.endGroup()
                            lblAuthUser.text =  username ? "Authorized as "+username : "Not authorized"
                            lblAuthUser.color = username ? "lime" : "red"

                        }

                    }

                }
                states: [
                    State {
                        name: "collapsed"
                        PropertyChanges {
                            target: authTwitchBody;
                            visible: false
                        }
                    },
                    State {
                        name: "expanded"
                        PropertyChanges {
                            target: authTwitchBody;
                            visible: true
                        }
                    }
                ]
            }



            Component.onCompleted: {
                authTwitchBody.state = "expanded"
            }
        }
        Page{
            id: pageConn
            title: qsTr("Connections")
            visible: true
            padding: 15
            font.family: "Sans-Serif"
            header: Label{
                font.pixelSize: 16
                text: pageConn.title
                height: 40
                padding: 10
            }
            ListView{
                id: listChannels
                width: parent.width
                height: listChannels.contentHeight
                model: servicesModel

                delegate: Component{
                    Row{
                        width: parent.width
                        height: 50
                        spacing: 15
                        ComboBox{
                            id: comboChannels
                            width: parent.width * 0.33
                            enabled: false
                            model: ListModel {
                                    id: lmComboChannels
                                    ListElement { text: "Twitch"; }
                                    ListElement { text: "Youtube";}
                                    ListElement { text: "Goodgame";}
                                    ListElement { text: "Hitbox";}
                                    ListElement { text: "Azubu";}
                                    ListElement { text: "Cybergame";}
                                }
                            Component.onCompleted: {
                                comboChannels.currentIndex = type
                            }
                        }
                        TextField{
                            id: tfChannelName
                            height: parent.height
                            text: channel
                            enabled: false
                        }
                        Image{
                            id: bttnReload
                            source: "icons/reload-24.png"
                            anchors.verticalCenter: parent.verticalCenter

                            MouseArea{
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: { parent.state='hover'}
                                onExited: { parent.state='normal'}
                                property string service_uuid: uuid
                                onClicked: {
                                    serviceManager.reconnectService(service_uuid)
                                }
                            }

                            states: [
                                State {
                                   name: "hover"
                                   PropertyChanges {
                                       target: bttnReload
                                       source: "icons/reload-24-lightblue.png"
                                   }
                                },
                                State {
                                    name: "normal"
                                    PropertyChanges {
                                        target: bttnReload
                                        source: "icons/reload-24.png"
                                    }
                                }
                            ]
                        }
                        Image{
                            id: bttnRemove
                            source: "icons/minus-5-24.png"
                            anchors.verticalCenter: parent.verticalCenter

                            MouseArea{
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: { parent.state='hover'}
                                onExited: { parent.state='normal'}
                                property string service_uuid: uuid
                                onClicked: {
                                    serviceManager.removeService(service_uuid)
                                }
                            }

                            states: [
                                State {
                                   name: "hover"
                                   PropertyChanges {
                                       target: bttnRemove
                                       source: "icons/minus-5-24-red.png"
                                   }
                                },
                                State {
                                    name: "normal"
                                    PropertyChanges {
                                        target: bttnRemove
                                        source: "icons/minus-5-24.png"
                                    }
                                }
                            ]
                        }
                    }
                }
                Component.onCompleted: {
                }
            }

            Row{
                id: rowService
                anchors.top: listChannels.bottom
                width: parent.width
                height: 50
                spacing: 15

                function addChannel(){
                    if(tfChannelName.text !== "" && settings.value(tfChannelName.text, -1, "Services") === comboChannels.currentIndex){
                        addChannelBusyIndicator.running = false
                        mainWindow.alertMessage("Channel is already added!")
                    }
                    else if(tfChannelName.text.length > 0){
                        switch(comboChannels.currentIndex){
                            case 0: //Twitch
                                secSettings.beginGroup("Twitch")
                                serviceManager.addService(
                                            comboChannels.currentIndex,
                                            tfChannelName.text,
                                            secSettings.value("twitch_username", "justinfan"+Math.random()*(99999999 - 10000000) + 10000000),
                                            secSettings.value("twitch_token", "randomString"))
                                secSettings.endGroup()
                            break;
                            case 1: //Youtube
                            case 2: //Goodgame
                            case 3: //Hitbox
                            case 4: //Azubu
                            case 5: //Cybergame
                                break;
                        }
                        tfChannelName.text = ""
                    }else{
                        addChannelBusyIndicator.running = false
                        mainWindow.alertMessage("Channel can't be empty!")
                    }
                }
                ComboBox{
                    id: comboChannels
                    width: parent.width * 0.33
                    model: ListModel {
                            id: lmComboChannels
                            ListElement { text: "Twitch"; }
                            ListElement { text: "Youtube";}
                            ListElement { text: "Goodgame";}
                            ListElement { text: "Hitbox";}
                            ListElement { text: "Azubu";}
                            ListElement { text: "Cybergame";}
                        }
                }
                TextField{
                    id: tfChannelName
                    height: parent.height
                    selectByMouse: true
                    placeholderText: "ChannelName"
                    Keys.onPressed: {
                        if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter){
                            rowService.addChannel()
                            addChannelBusyIndicator.running = true
                        }
                    }
                }
                Image{
                    id: bttnAdd
                    source: "icons/plus-5-24.png"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea{
                        anchors.fill: parent
                        onEntered: { parent.state='hover'}
                        onExited: { parent.state='normal'}

                        onClicked: {
                            rowService.addChannel()
                            addChannelBusyIndicator.running = true
                        }


                    }
                    states: [
                        State {
                           name: "hover"
                           PropertyChanges {
                               target: bttnAdd
                               source: "icons/plus-5-24-green.png"
                           }
                        },
                        State {
                            name: "normal"
                            PropertyChanges {
                                target: bttnAdd
                                source: "icons/plus-5-24.png"
                            }
                        }
                    ]
                }
                BusyIndicator{
                    id: addChannelBusyIndicator
                    running: false
                    width: 36
                    height: 36
                    anchors.verticalCenter: parent.verticalCenter
                    Connections {
                        target: serviceManager
                        onChannelNotExists: {
                            addChannelBusyIndicator.running = false
                        }
                    }

                    Connections{
                        target: servicesModel
                        onItemAdded: {
                            addChannelBusyIndicator.running = false
                        }
                    }

                    Connections{
                        target: mainWindow
                        onPopupClosed: {
                            addChannelBusyIndicator.running = false
                        }
                    }

                }
            }
        }
        Page{
            font.family: "Sans-Serif"
            title: qsTr("Webserver")
            padding: 15
            header: Label{
                font.pixelSize: 16
                text: parent.title
                height: 40
                padding: 10
            }
            Column{
                anchors.fill: parent
                Label{
                    text: "Chat URL"
                }
                TextField{
                    id: chatUrl
                    text: "http://localhost:8080"
                    width: parent.width
                    selectByMouse: true
                    readOnly: true
                }
            }
        }
        Page{
            id: pageAbout
            title: qsTr("About")
            font.family: "Sans-Serif"
            padding: 15
            header: Label{
                font.pixelSize: 16
                text: pageAbout.title
                height: 40
                padding: 10
            }
        }
    }
    function hex2rgba(hex){
        hex = hex.replace('#','');
        var red;
        var green;
        var blue;
        var alpha;
        if(hex.length > 6){
            alpha = parseInt(hex.substring(0,2), 16)/255;
            red  = parseInt(hex.substring(2,4), 16);
            green= parseInt(hex.substring(4,6), 16);
            blue = parseInt(hex.substring(6,8), 16);
        }
        else{
            alpha = 1
            red  = parseInt(hex.substring(2,4), 16);
            green= parseInt(hex.substring(4,6), 16);
            blue = parseInt(hex.substring(6,8), 16);
        }
        return {
         "r": red,
         "g": green,
         "b": blue,
         "a": alpha
        };
    }

    function invertColor(color){
        var r = color.a - color.r
        var g = color.a - color.g
        var b = color.a - color.b
      return Qt.rgba(r,g,b, color.a);
    }

    function findNearest(sourceValue, step){
        var rest = sourceValue % step
        if(rest <= step / 2){
            return sourceValue-rest
        }else{
            return sourceValue+rest
        }
    }

    function alertMessage(text){
        messageDialog.text = text
        messageDialog.visible = true
    }


    onClosing: {
        mainWindow.settingsShown = false
        settingsUI.destroy()
    }

}
