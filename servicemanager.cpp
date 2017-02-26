#include "servicemanager.h"
#include "twitch.h"
#include <QCoreApplication>
#include <QImageReader>
#include <QQuickWindow>
ServiceManager::ServiceManager(Settings *settings, ServicesModel *model, WebSocketServer *webSocketServer, QObject *parent) : QObject(parent)
{
    _webSocketServer = webSocketServer;
    _settings = settings;
    _serviceModel = model;
    _stackMessages = new QStack<QString>();
    _timerStackMessages = new QTimer(this);
    connect(_timerStackMessages, &QTimer::timeout, [=](){
        if(!_stackMessages->isEmpty()){
            QString const msg = _stackMessages->pop();
            _webSocketServer->sendMessage(msg);
            int delay = _settings->value("message_delay", 100, "Chat").toInt();
            if(_timerStackMessages->interval() != delay)
               _timerStackMessages->setInterval(delay);
        }
    });
    _networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    QUrl url("https://api.betterttv.net/2/emotes");
    request.setUrl(url);
    connect(_networkManager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply){
        QByteArray data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray emotes = jsonObject["emotes"].toArray();
//        qDebug() << emotes;
        for(QJsonValueRef value : emotes){
            _bttvEmotes.insert(value.toObject()["code"].toString(), value.toObject()["id"].toString());
        }
//        _networkManager->deleteLater();
        reply->deleteLater();
    });
    _networkManager->get(request);
    _timerStackMessages->start(_settings->value("message_delay", 100, "Chat").toInt());
}

ServiceManager::~ServiceManager()
{
//    delete _webSocketServer;
    delete _stackMessages;
}

QString ServiceManager::addService(const int type, const QString channel, const QString username, const QString token)
{
    QString service_uuid;
    switch(type)
    {
        case Service::TWITCH:
        {

            Twitch *twitch = new Twitch("irc.chat.twitch.tv", 6667, channel, username, token);
            service_uuid = twitch->getServiceUUid();
            _services.insert(service_uuid, twitch);
            connect(twitch, &Twitch::messageReceived, this, &ServiceManager::onMessage);
            connect(twitch, &Twitch::channelNotFound, [=](const QString uuid, const QString channel){
                Twitch *twitch = (Twitch*)_services.value(uuid);
                _settings->remove(twitch->getChannel(), "Services");
                _services.remove(uuid);
                emit channelNotExists(service_uuid, twitch->getChannel());
                twitch->deleteLater();
                emit alertMessage(QString("Channel '%1' does not exists!").arg(channel));
            });
            connect(twitch, &Twitch::channelExists, this, &ServiceManager::onChannelFound);

        }
        break;
        case Service::YOUTUBE:
        case Service::GOODGAME:
        case Service::AZUBU:
        case Service::CYBERGAME:
        case Service::HITBOX:
        case Service::NONE:
        case Service::SYSTEM:
        default:
            break;
    }


    return service_uuid;
}

void ServiceManager::removeService(const QString &channelId)
{
    Twitch *twitch = (Twitch*)_services.value(channelId);
    _services.value(channelId)->deleteLater();
    _services.remove(channelId);

    QString const key = _serviceModel->removeService(channelId);
    _settings->remove(key, "Services");
    twitch->deleteLater();
}

QHash<QString, Service *> ServiceManager::getServices()
{
    return _services;
}

void ServiceManager::reconnectService(const QString &channelId)
{
    _services.value(channelId)->reconnect();
}

void ServiceManager::addRandomMessage(const int type)
{
    QStringList listLoremIpsum = {"lorem", "ipsum", "dolor", "sit", "amet", "consectetuer", "adipiscing", "elit", "sed", "diam", "nonummy", "nibh", "euismod", "tincidunt", "ut", "laoreet", "dolore", "magna", "aliquam", "erat"};
    QString nickname = listLoremIpsum[rand() % (listLoremIpsum.length()-1)];
    QString text;
    for(int i = 0; i < rand() % 100 + 5; i++){
        text += listLoremIpsum[rand() % (listLoremIpsum.length()-1) + 0]+" ";
    }
    QHash<QString, QVariant> emptyHash;
    onMessage("", type, "SnakeChat", nickname, text, "receiver", emptyHash);
}

void ServiceManager::onMessage(QString uid, int type, QString channel, QString nickname, QString text, QString receiver, QHash<QString, QVariant> messageOptions)
{
    QString serviceIcon;
    //TODO: improve/simplify this declarations
    QString turboIconSpan = "";
    QString premiumIconSpan = "";
    QString subscriberIconSpan = "";
    QString modIconSpan = "";
    QString broadcasterIconSpan = "";
    QString bitsSpan = "";
    QString textColor = _settings->value("text_color", "#fff", "Message").toString();
    QString backgroundColor = Helper::argb2rgba(_settings->value("background_color", "#33000000", "Message").toString());
    QString time = _settings->value("show_time", true, "Chat").toBool() ? QTime::currentTime().toString() : "";
    QString nicknameColor = (messageOptions["color"].isNull() || messageOptions["color"] == "" || !_settings->value("twitch_colors", true, "Message").toBool()) ? _settings->value("nickname_color", "purple", "Message").toString() :   messageOptions["color"].toString();
    QString animation = QString(" animated %1").arg(_settings->value("animation", "slideUp", "Message").toString());
    switch(type)
    {
        case Service::TWITCH:
        {
//            Twitch * twitch = qobject_cast<Twitch*>(_services.value(uid));
            serviceIcon = "<img class='service-icon' src='/images/twitch.png' />";
            //TODO: improve code. Too much repeatable code
            if(messageOptions.value("turbo").toBool())
                turboIconSpan = "<img id='turboIconImage' src='"+messageOptions.value("turbo_badge").toString()+"' />";
            else if(!messageOptions.value("bits_badge").isNull()){
                bitsSpan = "<img id='bitsIconImage' src='"+messageOptions.value("bits_badge").toString()+"' />";
            }else if(!messageOptions.value("premium_badge").isNull()){
                premiumIconSpan = "<img id='premiumIconImage' src='"+messageOptions.value("premium_badge").toString()+"' />";
            }
            if(messageOptions.value("subscriber").toBool())
                subscriberIconSpan = "<img id='subscriberIconImage' title='"+messageOptions.value("sub_length").toString()+"-Month Subscriber' src='"+messageOptions.value("sub_badge").toString()+"' />";
            if(messageOptions.value("broadcaster").toBool())
                broadcasterIconSpan = "<img id='broadcasterIconImage' src='"+messageOptions.value("broadcaster_badge").toString()+"' />";
            else if(messageOptions.value("mod").toBool())
                modIconSpan = "<img id='moderatorIconImage' src='"+messageOptions.value("mod_badge").toString()+"' />";

        }
        break;
        case Service::YOUTUBE:
        case Service::GOODGAME:
        case Service::AZUBU:
        case Service::CYBERGAME:
        case Service::HITBOX:
        case Service::NONE:
        case Service::SYSTEM:
        default:
            break;

    }



    const QStringList words = text.split(" ");
    for(QString word : words){
        if(_bttvEmotes.contains(word)){
            QRegExp regex("\\b"+word+"\\b(?!\\')");
            text.replace(regex, "<img title='"+word+"' src='https://cdn.betterttv.net/emote/"+_bttvEmotes.value(word)+"/1x.png' />");
        }
    }

    QMap<QString,QString> matchedResult;
    while((matchedResult = Helper::matchUrl(text)).count() > 0){
        QMap<QString, QString>::iterator matchedLinkIter = matchedResult.begin();
        QString prefix;
        if(!matchedLinkIter.value().contains("http")){
            prefix = "http://";
        }else{
            prefix = "";
        }
        if(_settings->value("show_images", false, "Chat").toBool() && (matchedLinkIter.value().contains(".png")  || matchedLinkIter.value().contains(".jpg") | matchedLinkIter.value().contains(".jpeg") | matchedLinkIter.value().contains(".gif"))){
            text.replace(matchedLinkIter.value(), QString("<a href='%1%2' title='%1%2'><img src='%1%2' class='url-image' /></a>").arg(prefix).arg(matchedLinkIter.value()));
        }else{
            text.replace(matchedLinkIter.value(), QString("<a href='%1%2' title='%1%2'>Link to %3</a>").arg(prefix).arg(matchedLinkIter.value()).arg(matchedLinkIter.key()), Qt::CaseInsensitive);
        }

    }


    bool highlighted = false;
    if(text.toLower().contains(channel.toLower()))
        highlighted = true;
    if(highlighted)
        textColor = "#FF8C00";
    QString msg =
            "<div class='message "+animation+"' style='background-color:"+backgroundColor+";'>"\
                "<div class='msg-header'>"\
                    "<span class='badges'>"+serviceIcon+" "+broadcasterIconSpan+" "+modIconSpan+" "+subscriberIconSpan+" "+premiumIconSpan+" "+bitsSpan+" "+turboIconSpan+" <span class='nickname' style='color:"+nicknameColor+";text-align:left;'><b>"+nickname+":</b></span></span>"\
                    "<span class='time'>"+time+"</span>"\
                "</div>"\
                "<div class='msg-content'>"\
                    "<div valign='top' colspan='3'><p valign='top' class='message-text' style='color:"+textColor+"'>"+text+"</p><img src='https://cdn.betterttv.net/emote/567b00c61ddbe1786688a633/1x'/></div>"\
                    "<div></div>"\
                "</div>"\
            "</div>";

//    qDebug() << msg;
    _stackMessages->push_front(msg);
}

void ServiceManager::onChannelFound(const QString uuid, const QString channel, const int type)
{
    _settings->setValue(channel, type, "Services");
    ServiceChannel const newService(type, channel, uuid);
    _serviceModel->addService(newService);
}
