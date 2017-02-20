#include "service.h"

Service::Service(QString host, int port, QString channel, QString username, QString token, QObject *parent) :
    Socket(host, port, channel, username, token, parent)
{
    _channel = channel;
    _username = username;
    _token = token;
    _serviceUid = QUuid::createUuid().toString();
}

QString Service::getServiceUUid(){
    return _serviceUid;
}
