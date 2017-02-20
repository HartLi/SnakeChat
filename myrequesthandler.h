#ifndef MYREQUESTHANDLER_H
#define MYREQUESTHANDLER_H
#include "encryptedsettings.h"
#include "settings.h"

#include <httpconnection.h>
#include <httprequesthandler.h>
#include "httprequest.h"
#include "httpresponse.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
using namespace HobrasoftHttpd;
class MyRequestHandler : public HttpRequestHandler
{
    Q_OBJECT
public:
    MyRequestHandler(EncryptedSettings *settings, HttpConnection *connection);
    void service(HttpRequest *request, HttpResponse *response);
protected:
    EncryptedSettings *_settings;
private:
    QNetworkAccessManager *_manager;
public Q_SLOTS:
};

#endif // MYREQUESTHANDLER_H
