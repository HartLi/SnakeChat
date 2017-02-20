#include "twitch.h"

Twitch::Twitch(QString host, int port, QString channel, QString nickname, QString token, QObject *parent) :
    Service(host, port, channel, nickname, token, parent)
{
    _hostValidated = false;
    this->evaluateChannelBasedHostAddress();
    this->loadBadges();

}

Twitch::~Twitch()
{
}

bool Twitch::getHostValidated()
{
    return _hostValidated;
}

void Twitch::setHostValidated(bool hostValidated)
{
    _hostValidated = hostValidated;
}

 void Twitch::loadChannelInfo()
{
    _networkManager = new QNetworkAccessManager(this);
    QUrl url("https://api.twitch.tv/kraken/streams/"+_channel);
    QNetworkRequest request;
    request.setRawHeader("Authorization", "OAuth %1 "+_token.toUtf8());
    request.setUrl(url);
    connect(_networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply*reply){
        if(reply->error()){
            qDebug() << "Error: " << reply->errorString();
            return;
        }
        QString data = (QString) reply->readAll();
         qDebug() << "Stream info received. Parsing...";
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        if(jsonObject["stream"] != NULL){
            _isLive = true;
            _viewersCount = jsonObject["viewers"].toInt();
        }else{
            _isLive = false;
        }
        reply->deleteLater();
    });
    _networkManager->get(request);  // GET
}

QString Twitch::getChannel() const
{
    return _channel;
}

void Twitch::setChannel(const QString &channel)
{
    _channel = channel;
}

QHash<QString, QMap<QString, QVariant> > Twitch::getBadges() const
{
    return _badges;
}

void Twitch::setBadges(const QHash<QString, QMap<QString, QVariant> > &badges)
{
    _badges = badges;
}

void Twitch::evaluateChannelBasedHostAddress()
{
    _networkManager = new QNetworkAccessManager(this);
    QUrl url("https://api.twitch.tv/api/channels/"+_channel+"/chat_properties");
    QNetworkRequest request;
    request.setRawHeader("Authorization", "OAuth "+_token.toUtf8());
    request.setUrl(url);
    connect(_networkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply){
        if(!reply->error()){
            emit channelExists(_serviceUid, _channel, Service::TWITCH);
            QString data = (QString) reply->readAll();

             qDebug() << "Response received. Parsing...";
            QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
            _channelId = jsonObject["_id"].toInt();
            QJsonArray servers = jsonObject["chat_servers"].toArray();
            QString newHost = servers[rand() % servers.count()].toString();
            QString newIp = newHost.split(":")[0];
            QString newPort = newHost.split(":")[1];
            qDebug() << "data: " << data;
            _host = newIp;
            _port = newPort.toInt();
            setHostValidated(true);
            qDebug() << "Obtained chat-server for "<< _channel << "->IP: " << _host << " Port: " << _port;
            this->connnectToServer();
            QObject::connect(this, SIGNAL(connected()), this, SLOT(connectedToIRC()));




            //Load Sub Badges//
            _networkManager = new QNetworkAccessManager(this);
            QUrl url(QString("https://badges.twitch.tv/v1/badges/channels/%1/display").arg(_channelId));
            QNetworkRequest request;
            request.setRawHeader("Authorization", "OAuth "+_token.toUtf8());
            request.setUrl(url);
            try{
                _networkManager->get(request);  // GET
            }catch(std::exception &ex){
                qDebug() << "Catched exception on sub badges: " << ex.what();
            }

            connect(_networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply){
                QString data = (QString) reply->readAll();
                 qDebug() << "Sub badges received. Parsing...";
                QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
                QJsonObject jsonObject = jsonResponse.object();
                if(!jsonObject["badge_sets"].toObject().isEmpty()){
                    QVariantMap varMapVersions = jsonObject["badge_sets"].toObject().toVariantMap();
                    for(QVariantMap::const_iterator iter = varMapVersions.begin(); iter != varMapVersions.end(); ++iter) {
                        _badges.insert(iter.key(), iter.value().toMap().value("versions").toMap());
//                        qDebug() << iter.value();
//                        qDebug() << iter.value().toMap();
//                        qDebug() << iter.value().toHash();
                    }
                }
            });
        }
        else{
            qDebug() << "Error" << reply->errorString();
            if(reply->error() == QNetworkReply::ContentNotFoundError){
                emit channelNotFound(_serviceUid, _channel);
            }
            this->disconnectFromServer();
        }
        reply->deleteLater();
    });
    try{
        _networkManager->get(request);  // GET
    }
    catch(std::exception &){
        qDebug() << "Exception happened";
    }

}

void Twitch::parseMessage(QString msg)
{
    QString nickname;
    QString color;
    QString receiver;
    QStringList privMsgList = msg.split("PRIVMSG #"+_channel.toLower() + " :");
    QStringList msgList = privMsgList[0].split( ";", QString::SkipEmptyParts);
    QMap<QString, QString> msgMap;
    for( int i = 0; i < msgList.size(); i++ )
            msgMap[msgList[i].split( "=" )[0]] = msgList[i].split( "=" )[1];

    msgMap["messageText"] = privMsgList[1];
    nickname = msgMap["display-name"];
    if(nickname == ""){
        int start = msgMap["user-type"].indexOf(":")+1;
        int end = msgMap["user-type"].indexOf("!");
        int length=end-start;
        nickname = msgMap["user-type"].mid(start, length);
//        qDebug() << nickname;
    }
    msgMap["messageText"].replace(msgMap["messageText"].indexOf("\r"), 2, " ");
    msgMap["messageText"].replace(msgMap["messageText"].indexOf("\n"), 2, " ");
//    qDebug() << _badges;
    QHash<QString, QVariant> options;
    options.insert("color", msgMap["color"]);
    options.insert("subscriber", msgMap["subscriber"] == "1" ? true : false);
    options.insert("turbo", msgMap["turbo"] == "1" ? true : false);
    if(msgMap["@badges"].contains("turbo/1")){
        options.insert("turbo_badge", _badges.value("turbo").value("1").toMap().value("image_url_1x"));
    }
    options.insert("mod", msgMap["mod"] == "1" ? true : false);
    if(options.value("mod").toBool()){
        options.insert("mod_badge", _badges.value("moderator").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("broadcaster/1")){
        options.insert("broadcaster", true);
        options.insert("broadcaster_badge", _badges.value("broadcaster").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("staff/1")){
        options.insert("staff_badge", _badges.value("staff").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("admin/1")){
        options.insert("admin_badge", _badges.value("admin").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("premium/1")){
        options.insert("premium_badge", _badges.value("premium").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("global_mod/1")){
        options.insert("global_mod_badge", _badges.value("global_mod").value("1").toMap().value("image_url_1x"));
    }
    if(msgMap["@badges"].contains("bits")){
        QStringList badges = msgMap["@badges"].split(",");
        QRegExp regExp;
        regExp.setPatternSyntax(QRegExp::RegExp2);
        regExp.setPattern("^bits\\/\\d+$");
        int index = badges.indexOf(regExp);
        if(index > -1){
            QString rawString = badges.value(index);
            QString id = rawString.split("/")[1];
            if(_badges.value("bits").count() > 0){
                options.insert("bits_badge", _badges.value("bits").value(id).toMap().value("image_url_1x"));
            }
        }
    }
    if(options.value("subscriber").toBool() == true){
        QStringList badges = msgMap["@badges"].split(",");
        QRegExp regExp;
        regExp.setPatternSyntax(QRegExp::RegExp2);
        regExp.setPattern("^subscriber\\/\\d+$");
        int index = badges.indexOf(regExp);
        if(index > -1){
            QString subAndBadgeId = badges.value(index);
            QString subBadgeId = subAndBadgeId.split("/")[1];
            if(_badges.value("subscriber").count() > 0){
                options.insert("sub_badge", _badges.value("subscriber").value(subBadgeId).toMap().value("image_url_1x"));
                options.insert("sub_length", subBadgeId);
            }
        }
    }
    QString text = msgMap["messageText"];

    if(msgMap["emotes"] != ""){
        QHash<QString,QString> toReplace;
        QStringList emotesList = msgMap["emotes"].split("/");
        for(QString emote : emotesList) {
            QStringList list = emote.split(":");
            QString id = list[0];
            QStringList range = list[1].split(",")[0].split("-");
            QString emoteText;
//            qDebug() << range;
            for(int i = range[0].toInt(); i <= range[1].toInt(); i++){
                emoteText.append(text[i]);
            }
            toReplace.insert(id, emoteText);
        }
//        qDebug() << toReplace;
        QHash<QString, QString>::iterator iter;
        for (iter = toReplace.begin(); iter != toReplace.end(); ++iter){
            QString emoteUrl = "<img alt='"+iter.value()+"' title='"+iter.value()+"' style='vertical-align: middle' src='http://static-cdn.jtvnw.net/emoticons/v1/"+iter.key()+"/1.0' />";
//            qDebug() << emoteUrl;
            const QString oldText = text;
            QString rxString = QString("\\b%1\\b").arg(iter.value());
            QRegExp regex(rxString);
            text.replace(regex, emoteUrl);
            if(oldText == text){
                text.replace(iter.value(), emoteUrl);
            }
        }
    }

    emit messageReceived(this->getServiceUUid(), Service::TWITCH,
                        _channel,
                        nickname,
                        text,
                        receiver,
                        options);
//    qDebug() << msg;
//    qDebug() << text;
}

void Twitch::loadBadges()
{
    _networkManager = new QNetworkAccessManager(this);
    QUrl url("https://badges.twitch.tv/v1/badges/global/display");
    QNetworkRequest request;
//    request.setRawHeader("Authorization", "OAuth "+_token.utf16());
    request.setUrl(url);
    try{
       _networkManager->get(request);  // GET
    }
    catch(std::exception &){
    }
    connect(_networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply){


        QString data = (QString) reply->readAll();
         qDebug() << "Stram badges received. Parsing...";
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();

        if(!jsonObject["badge_sets"].toObject().isEmpty()){
            QVariantMap varMap = jsonObject["badge_sets"].toObject().toVariantMap();
            for(QVariantMap::const_iterator iter = varMap.begin(); iter != varMap.end(); ++iter) {
                QMap<QString, QVariant> mapVersions = iter.value().toMap().value("versions").toMap();
                _badges.insert(iter.key(),  mapVersions);
            }
        }else{
            qDebug() << "No global badges were found";
        }

        reply->deleteLater();

    });
}

void Twitch::dataReceived()
{
    QString line;
    QByteArray  join;
    while(_socket->canReadLine()){
        line = _socket->readLine();
        if(line.contains("ACK") && line.contains("membership")){
                qDebug() << "Membership acknowledged. Sending JOIN request...";
                join = QString("JOIN #"+_channel.toLower()+"\r\n").toUtf8();
                _socket->write(join);
        }
        if(line.contains("PING")){
            _socket->write("PONG :tmi.twitch.tv\r\n");
            qDebug() << "Responded with PONG";
        }
        if(line.contains("twitchnotify")){
            qDebug() << line;
        }
        else if(line.contains("PRIVMSG")){
            parseMessage(line);
        }
//        qDebug() << line;
    }
}

void Twitch::connectedToIRC()
{
    QByteArray  nick = QString("NICK "+_username.toLower()+"\r\n").toUtf8();
    QByteArray  token = QString("PASS oauth:"+_oAuthToken+"\r\n").toUtf8();

    _socket->write(token);
    _socket->write(nick);
    _socket->write("CAP REQ :twitch.tv/membership\r\n");
    _socket->write("CAP REQ :twitch.tv/tags\r\n");
    _socket->write("CAP REQ :twitch.tv/commands\r\n");

    if(!_socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << _socket->errorString();
    }
}
