#include "websocketserver.h"
#include <QWebSocket>
WebSocketServer::WebSocketServer(int port, QObject* parent) : QObject(parent)
{
    webSocketServer = new QWebSocketServer("ws://127.0.0.1", QWebSocketServer::NonSecureMode, this);
    if(webSocketServer->listen(QHostAddress::Any, port)){
        qDebug() << "Listening to localhost:"<< port << endl;
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(webSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }
}


void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = webSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

void WebSocketServer::processTextMessage(QString message)
{
    foreach(QWebSocket *s, m_clients)
    {
        if (s) {
           s->sendTextMessage(message);
        }
    }
}

void WebSocketServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient)
    {
       m_clients.removeAll(pClient);
       pClient->deleteLater();
    }
}


WebSocketServer::~WebSocketServer()
{
    webSocketServer->close();
    deleteLater();
}

void WebSocketServer::sendMessage(const QString msg)
{
    foreach(QWebSocket *s, m_clients)
    {
        if (s) {
           s->sendTextMessage(msg);
        }
    }
}

