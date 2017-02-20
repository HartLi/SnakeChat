#include "socket.h"
#include <QDebug>

void Socket::dataReceived()
{
    qDebug() << "Data received";
}

void Socket::errorReceived()
{
   qDebug() << _socket->errorString() << endl;
}

Socket::Socket(QString host, int port, QString channel, QString username, QString token, QObject *parent) : QObject(parent)
{
    _host = host;
    _port = port;
    _oAuthToken = token;
    _socket = new QTcpSocket(this);

    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QObject::connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(errorReceived()));
    QObject::connect(_socket, SIGNAL(connected()), this,  SLOT(onConnect()));
    QObject::connect(_socket, SIGNAL(disconnected()), this,  SLOT(onDisconnect()));
}

bool Socket::connnectToServer()
{
    qDebug() << "Trying to connect to" << _host << ":" << _port << "..." << endl;
    _socket->connectToHost(_host, _port);

    return true;
}

bool Socket::disconnectFromServer()
{
    QObject::disconnect(_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QObject::disconnect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,  SLOT(errorReceived()));
    QObject::disconnect(_socket, SIGNAL(connected()), this,  SLOT(onConnect()));
    QObject::disconnect(_socket, SIGNAL(disconnected()), this,  SLOT(onDisconnect()));
    _socket->flush();
    _socket->disconnectFromHost();
    _socket->close();
    return true;
}

bool Socket::disconnect()
{
    _socket->disconnectFromHost();

    return _socket->ConnectedState == 0;
}


bool Socket::reconnect(){
    this->disconnect();
    this->connnectToServer();

    return _socket->ConnectedState >= 4;
}

Socket::~Socket()
{
    disconnectFromServer();
    delete _socket;
}

void Socket::onConnect()
{
    emit connected();
    qDebug() << "Successfully connected to " << _host << ":" << _port << " Server";
}

void Socket::onDisconnect()
{
    emit disconnected();
    qDebug() << "Disconnected from " << _host << ":" << _port << " Server";
    reconnect();
}

