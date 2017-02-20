#include "myrequesthandler.h"


MyRequestHandler::MyRequestHandler(EncryptedSettings *settings, HttpConnection *connection) : HttpRequestHandler(connection)
{
    _settings = settings;
}

void MyRequestHandler::service(HttpRequest *request, HttpResponse *)
{
    QString path = request->path();
    if(path == "/twitch_oauth"){
        QJsonDocument document =  QJsonDocument::fromJson(request->body());
        QJsonObject obj = document.object();
        if(!obj.value("error").isUndefined() && !obj.value("error").isNull()){
            qDebug() << "Error: " << obj.value("error").toString() << endl;
            qDebug() << "Status: " << obj.value("status").toString() << endl;
            qDebug() << "Message: " << obj.value("message").toString() << endl;
            return;
        }else{
            QString oauth_token =  obj.value("token").toString();
            if(!oauth_token.isEmpty()){
                _manager = new QNetworkAccessManager(this);
                QNetworkRequest networkRequest(QUrl("https://api.twitch.tv/kraken/user"));
                networkRequest.setRawHeader("Accept", "application/vnd.twitcht.v5+json");
                networkRequest.setRawHeader("Client-ID", _settings->value("clientId","").toByteArray());
                std::string authHeader = QString("OAuth %1").arg(oauth_token).toStdString();
                networkRequest.setRawHeader("Authorization", QByteArray::fromStdString(authHeader));
                _manager->get(networkRequest);

                connect(_manager, &QNetworkAccessManager::finished,this,
                [=](QNetworkReply*reply){
                    QJsonDocument document =  QJsonDocument::fromJson(reply->readAll());
                    QJsonObject obj = document.object();
                    if(!obj.value("error").isUndefined() && !obj.value("error").isNull()){
                        qDebug() << "Error: " << obj.value("error").toString() << endl;
                        qDebug() << "Status: " << obj.value("status").toString() << endl;
                        qDebug() << "Message: " << obj.value("message").toString() << endl;
                        return;
                    }else{
                        QString username =  obj.value("name").toString();
                        _settings->beginGroup("Twitch");
                        _settings->setValue("twitch_username", username);
                        _settings->setValue("twitch_token", oauth_token);
//                        qDebug() << "Twitch username: " << _settings->value("twitch_username").toString() << endl;
//                        qDebug() << "Twitch token: " << _settings->value("twitch_token").toString() << endl;
                        _settings->endGroup();

                    }
                    reply->deleteLater();
                    _manager->deleteLater();
                });
            }else{
                qDebug() << "No token returned by Twitch API :(";
                return;
            }
        }
    }
}
