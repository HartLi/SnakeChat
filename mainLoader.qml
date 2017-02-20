import QtQml 2.2
import QtQuick 2.7

Item {
    Loader {
        id: mainWindowLoader
        active: false
        source: "qrc:/main.qml"
        asynchronous: false

        onLoaded: {
            item.visible = true;
            splashScreenLoader.item.visible = false;
            splashScreenLoader.source = "";
            splashScreenLoader.destroy()

        }
    }


    Loader {
        id: splashScreenLoader
        source: "qrc:/Splash.qml"
        property int timeoutInterval: 2000
        signal timeout

        Timer {
            id: timer
            interval: splashScreenLoader.timeoutInterval; running: true; repeat: false
            onTriggered: {
                mainWindowLoader.active = true;
                splashScreenLoader.timeout()
                timer.destroy()
            }
        }
    }
}
