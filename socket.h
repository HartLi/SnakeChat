#ifndef IRC_H
#define IRC_H

#include <QNetworkReply>
#include <QTcpSocket>
#include <QObject>
class Socket : public QObject
{
    Q_OBJECT
protected:
    QTcpSocket *_socket;
    QString _host;
    QString _oAuthToken;
    int _port;
private slots:
        virtual void dataReceived();
        void errorReceived();
        virtual void onConnect();
        void onDisconnect();
//        virtual void onResult(QNetworkReply*);

public:
    explicit Socket(QString host, int port, QString channel, QString nickname, QString token, QObject *parent =0);
    bool connnectToServer();
    bool disconnectFromServer();
    bool reconnect();
    bool disconnect();
    ~Socket();
Q_SIGNALS:
    void disconnected();
    void connected();
private:

};

#endif // IRC_H
