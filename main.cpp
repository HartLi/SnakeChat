#include "encryptedsettings.h"
#include "servicemanager.h"
#include "settings.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <httpsettings.h>
#include "myrequesthandler.h"
#include <QQmlApplicationEngine>
#include <websocketserver.h>
#include "httpserver.h"
#include "requestmapper.h"
#include <QQmlContext>
#include <fstream>
#include <iostream>
int main(int argc, char *argv[])
{

//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QtWebEngine::initialize();
    QQmlApplicationEngine engine;
    Settings settings(QCoreApplication::applicationDirPath()+"/config/snake.ini", QSettings::IniFormat, &app);
    settings.setValue("clientId", "pcjraf3wf7s8oyeea2l6obepbs9xnh");
    QSettings listenerSettings(QCoreApplication::applicationDirPath()+"/config/webserver.ini", QSettings::IniFormat,&app);
    EncryptedSettings *encryptedSettings = new EncryptedSettings(settings.value("clientId","pcjraf3wf7s8oyeea2l6obepbs9xnh").toString(), QCoreApplication::applicationDirPath()+"/config/auth.ini", QSettings::IniFormat, &app);
    listenerSettings.beginGroup("http");
    listenerSettings.setValue("port", 8080);
    listenerSettings.setValue("address", "127.0.0.1");
    listenerSettings.setValue("root", QString("%1/web/").arg(QCoreApplication::applicationDirPath()));
    const int messagesPort = settings.value("port", 12345, "MessagesSocket").toInt();
    WebSocketServer websocket(messagesPort, &app);
    ServicesModel *servicesModel = new ServicesModel(&app);
    ServiceManager *serviceManager = new ServiceManager(&settings, servicesModel, &websocket, &app);

    using namespace HobrasoftHttpd;
    HttpSettings const httpSettings(&listenerSettings, 0);
    new MyHttpd(encryptedSettings,&settings, &httpSettings, &app);
    QStringList const channels = settings.readGroupKeys("Services");
    Q_FOREACH(const QString channel, channels){
        settings.beginGroup("Services");
        const int type = settings.value("channel").toInt();
        settings.endGroup();
        encryptedSettings->beginGroup("Twitch");
        QString const username = encryptedSettings->value("twitch_username", QString("justinfan%1").arg(rand() % 999999999 + 111111111)).toString();
        QString const token = encryptedSettings->value("twitch_token", "random_token").toString();
        encryptedSettings->endGroup();
        switch (type) {
            case Service::TWITCH:
                serviceManager->addService(type,
                    channel,
                    username,
                    token);
                break;
            default:
                break;
        }
    }
    engine.rootContext()->setContextProperty("applicationDirPath", QCoreApplication::applicationDirPath());
    engine.rootContext()->setContextProperty("websocket", &websocket);
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.rootContext()->setContextProperty("serviceManager", serviceManager);
    engine.rootContext()->setContextProperty("secSettings", encryptedSettings);
    engine.rootContext()->setContextProperty("servicesModel", servicesModel);
    engine.rootContext()->setContextProperty("messagesPort", messagesPort);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    QObject::connect(serviceManager, SIGNAL(alertMessage(QVariant)), window, SLOT(alertMessage(QVariant)));

    return app.exec();

}
