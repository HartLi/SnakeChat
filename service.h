#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <socket.h>
#include <quuid.h>
class Service : public Socket
{
    Q_OBJECT
public:
    explicit Service(QString host, int port, QString channel, QString username, QString token, QObject *parent = 0);
    enum{
        TWITCH = 0,
        YOUTUBE,
        GOODGAME,
        HITBOX,
        AZUBU,
        CYBERGAME,
        NONE,
        SYSTEM
    };
    QString getServiceUUid();
protected:
    int _viewersCount;
    bool _subscriber;
    bool _follower;
    QString _serviceUid;
    QHash<QString, QString> _emotes;
    QString _channel;
    QString _username;
    QString _token;
    bool _online;
    int _chattersCount;
    int _messagesCount;
Q_SIGNALS:
    void channelNotFound(const QString uuid, const QString channel);
    void channelExists(const QString uuid, const QString channel, const int type);
public slots:
};

#endif // SERVICE_H
