#ifndef WEBSCOCKETSERVER_H
#define WEBSCOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QtCore/QList>
#include <QtCore/QByteArray>
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT
private:
    QWebSocketServer *webSocketServer;
    QList<QWebSocket*> m_clients;
private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
Q_SIGNALS:
    void closed();
public:
    WebSocketServer(int port, QObject *parent=Q_NULLPTR);
    ~WebSocketServer();
    Q_INVOKABLE void sendMessage(const QString);
};

#endif // WEBSCOCKETSERVER_H
