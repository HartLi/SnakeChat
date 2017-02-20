#ifndef TWITCH_H
#define TWITCH_H

#include <service.h>
#include <socket.h>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <stdio.h>
#include <QMap>
#include <QObject>
class Twitch : public Service
{
    Q_OBJECT
public:
    Twitch(QString host, int port, QString channel, QString nickname, QString token, QObject *parent = 0);
    ~Twitch();
    bool getHostValidated();
    void setHostValidated(bool hostValidated);

    void loadChannelInfo();
    QString getChannel() const;
    void setChannel(const QString &channel);

    bool getIsLive() const;
    void setIsLive(bool isLive);

    QHash<QString, QMap<QString, QVariant> > getBadges() const;
    void setBadges(const QHash<QString, QMap<QString, QVariant> > &badges);
Q_SIGNALS:
    void messageReceived(QString uid, int service, QString channel, QString nickname, QString text, QString receiver, QHash<QString, QVariant> messageOptions);
private:
    QString _validHostAddress;
    bool _hostValidated;
    QHash<QString,QMap<QString, QVariant>> _badges;

    void evaluateChannelBasedHostAddress();
    void parseMessage(QString msg);
    void loadBadges();
    bool _isLive;
    int _channelId = 0;
    QNetworkAccessManager *_networkManager;

private Q_SLOTS:
    void dataReceived();
    void connectedToIRC();
};

#endif // TWITCH_H
