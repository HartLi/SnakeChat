import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0
import QtWebEngine 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: settings.value("window_width", 315)
    height: settings.value("window_height", 475)
    x: settings.value("window_x", 100)
    y: settings.value("window_y", 100)
    title: qsTr("Snake Chat")
    minimumWidth: 100
    minimumHeight: 100
    color: settings.value("background_color", Material.color(Material.BlueGrey, Material.Shade900), "WindowAppearance")
    flags: settings.value("native_titlebar", "false") === "false" ? Qt.Window | Qt.FramelessWindowHint : Qt.Window | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint

    property bool settingsShown: false
    property variant settingsWindow
    property variant settingsComponent
    signal popupClosed

    header:LinearGradient {
        id: rectHeader
        height: 30
        start: Qt.point(0, 0)
        end: Qt.point(0, viewSwipe.height)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#55000000"  }
            GradientStop { position: 1.0; color: mainWindow.color}
        }
        Row {
            anchors.top: parent.top
            width: mainWindow.width
            spacing: 20
            height: 25
            padding: {
                right: 10
                left: 10
                top: 7
            }

            layoutDirection: "RightToLeft"
            Image {
                id: closeIcon
                source: "icons/x-mark-16.png"
                opacity: 0.5
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        close()
                    }
                }
                OpacityAnimator{
                        id: closeAnim
                        target: closeIcon
                        from: 0.2
                        to: 0.5
                        duration: 50
                }
            }
            Image {
                id: minimizeIcon
                anchors.bottom: parent.bottom
                opacity: 0.5
                source: "icons/minus-2-16.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        miniAnim.start()
                        mainWindow.visibility = "Minimized"
                    }
                }
                OpacityAnimator{
                        id: miniAnim
                        target: minimizeIcon
                        from: 0.2
                        to: 0.5
                        duration: 50
                }
            }
            Image {
                id: settingsIcon
                opacity: 0.5
                source: "icons/settings-4-16.png"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        settingsAnim.start()
                        if(!settingsShown) {
                            settingsWindow = settingsComponent.createObject()
                            settingsWindow.show()
                            settingsWindow.raise()
                            settingsShown = settingsWindow.visible
                        }
                    }
                }
                OpacityAnimator{
                    id: settingsAnim
                    target: settingsIcon
                    from: 0.2
                    to: 0.5
                    duration: 50
                }
            }
            MouseArea {
                width: mainWindow.width
                height: 25
                property variant clickPos: "1,1"

                onPressed: {
                    clickPos = Qt.point(mouse.x,mouse.y)
                }

                onPositionChanged: {
                    var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                    var new_x = mainWindow.x + delta.x
                    var new_y = mainWindow.y + delta.y
                    if (new_y <= 0)
                        mainWindow.visibility = Window.Maximized
                    else
                    {
                        if (mainWindow.visibility === Window.Maximized)
                            mainWindow.visibility = Window.Windowed
                        mainWindow.x = new_x
                        mainWindow.y = new_y
                    }
                }
            }

        }

    }
    Page {
        id: chatPage
        anchors.top: rectHeader.bottom
        anchors.bottom: viewSwipe.top
        anchors.fill: parent
        background: Rectangle{
                anchors.fill: chatPage
                color: mainWindow.color

            }

        WebEngineView{
            id: webView
//            profile.httpCacheType: WebEngineProfile.NoCache
            backgroundColor: mainWindow.color
            anchors.fill: parent
            settings.autoLoadImages: true
            settings.localContentCanAccessRemoteUrls: true
            settings.localContentCanAccessFileUrls: true
            settings.javascriptEnabled: true
            settings.allowRunningInsecureContent: true
            settings.localStorageEnabled: true
//            url: applicationDirPath + "/web/index.html"
            url: "http://localhost:8080/index.html"
            activeFocusOnPress: true
//            settings.screenCaptureEnabled: true
            settings.defaultTextEncoding: "utf-8"
            onContextMenuRequested: {
                request.accepted = true
            }
            onJavaScriptConsoleMessage: {
                console.log(message)
            }
            onNavigationRequested: {
                if(request.navigationType === 0){
                    Qt.openUrlExternally(request.url.toString())
                    request.action = WebEngineView.IgnoreRequest
                }else if(request.url.toString().indexOf("localhost") === -1){
                    request.action = WebEngineView.IgnoreRequest
                }
            }
            onCertificateError: {
                error.ignoreCertificateError()
            }
        }


    }

    footer:SwipeView {
        id: viewSwipe
        height: 30
        currentIndex: 1
        anchors.top: chatPage.bottom
        background: LinearGradient{
            id: linGradient
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(0, viewSwipe.height)
            gradient: Gradient {
                GradientStop { position: 0.0; color: mainWindow.color }
                GradientStop { position: 1.0; color: "#55000000" }
            }
        }

        Item {
            Text{
                color: "white"
                text: "Debug panel"
            }

            Row{
                padding: 5
                Button{
                    height: 30
                    text: "Add message"
                    onClicked: {
                        serviceManager.addRandomMessage(0)
                    }
                }
            }
        }
        Item{
          id: resizeItem
          z: 10
          Rectangle {
              anchors.bottom: parent.bottom
              anchors.right: parent.right
              width: 16
              height: 16
              color: "transparent"
              z: 1
              Image {
                  id: resizeIcon
                  opacity: 0.075
                  source: "icons/resize-8-16.png"
              }
              MouseArea {
                  cursorShape: Qt.SizeFDiagCursor
                  anchors.fill: parent
                  onPositionChanged: {
                      if(mainWindow.width + mapToItem(resizeItem,mouseX,mouseY).x-resizeItem.width >= 100)
                          mainWindow.width += mapToItem(resizeItem,mouseX,mouseY).x-resizeItem.width
                      if(mainWindow.height + mapToItem(resizeItem,mouseX,mouseY).y-resizeItem.height >= 100)
                          mainWindow.height += mapToItem(resizeItem,mouseX,mouseY).y-resizeItem.height
                  }
              }
          }
        }
    }

    onClosing: {
        if(settingsWindow)
            settingsWindow.destroy()
        saveSettings()
        webView.profile.clearHttpCache();
    }

    Component.onCompleted: {
        mainWindow.visible = true
        if(settings.value("topmost", false) === "true")
            mainWindow.flags +=  Qt.WindowStaysOnTopHint
        settingsComponent = Qt.createComponent("SettingsUI.qml")
    }


    function saveSettings(){
        settings.setValue("window_x", mainWindow.x)
        settings.setValue("window_y", mainWindow.y)
        settings.setValue("window_width", mainWindow.width)
        settings.setValue("window_height", mainWindow.height)
        console.log("saving settings")
    }

    function alertMessage(text){
        popup.text = text
        popup.open()
    }
    Popup {
        id: popup
        property string text: ""

        parent: ApplicationWindow.overlay
        modal: true
        background: Rectangle{
            border.color: Material.color(Material.BlueGrey, Material.Shade300)
            color: mainWindow.color
        }


        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 200
        height: 120
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        Label{
           id: lblMessage
           width: parent.width
           text: popup.text
           horizontalAlignment: Label.AlignHCenter
           wrapMode: "WordWrap"
        }
        Button{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            id: closeButton
            height: 35
            text: "Ok"
            onClicked: {
                popup.close()
                popupClosed()
            }
        }
    }
}
