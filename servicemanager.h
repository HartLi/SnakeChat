#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "service.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <QTimer>
#include "websocketserver.h"
#include "helper.h"
#include "settings.h"
#include "servicesmodel.h"
#include <QStack>
class ServiceManager : public QObject
{
    Q_OBJECT
public:
    explicit ServiceManager(Settings *settings, ServicesModel *model, WebSocketServer *webSocketServer, QObject *parent = 0);
    ~ServiceManager();
    Q_INVOKABLE QString addService(const int type, const QString channel, const QString username, const QString token);
    Q_INVOKABLE void removeService(const QString &channelId);
    Q_INVOKABLE QHash<QString, Service *> getServices();
    Q_INVOKABLE void reconnectService(const QString &channelId);
    Q_INVOKABLE void addRandomMessage(const int type = Service::TWITCH);
private:
    QHash<QString, Service *> _services;
    WebSocketServer *_webSocketServer;
    Settings *_settings;
    ServicesModel *_serviceModel;
    QStack<QString> *_stackMessages;
    QTimer *_timerStackMessages;
    QHash<QString,QString> _bttvEmotes;
    QNetworkAccessManager *_networkManager;
signals:
    void alertMessage(QVariant text);
    void channelNotExists(const QVariant uuid, const QVariant channel);
public slots:
    void onMessage(QString uid, int service, QString channel, QString nickname, QString text, QString receiver, QHash<QString, QVariant> messageOptions);
    void onChannelFound(const QString uuid, const QString channel, const int type);

};

#endif // SERVICEMANAGER_H
