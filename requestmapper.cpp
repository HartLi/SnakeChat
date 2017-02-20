#include "encryptedsettings.h"
#include "requestmapper.h"
#include <staticfilecontroller.h>

MyHttpd::MyHttpd(EncryptedSettings *enSettings, Settings *settings, const HttpSettings *httpsettings, QObject *parent) : HttpServer(httpsettings, parent)
{
    _enSettings = enSettings;
    _settings = settings;
}

HttpRequestHandler *MyHttpd::requestHandler(HttpConnection *connection) {
    return new RequestMapper(_enSettings, _settings, connection);
}

RequestMapper::RequestMapper(EncryptedSettings *enSettings, Settings *settings, HttpConnection *connection) : HttpRequestHandler(connection)
{
    _enSettings = enSettings;
    _settings = settings;
    _staticController = new StaticFileController(connection);
}

void RequestMapper::service(HttpRequest *request, HttpResponse *response) {
    if(request->path() == "/" || request->path() == "/index.html"){
        response->setCookie(HttpCookie("messages_port", _settings->value("port", 12345, "MessagesSocket").toString(), 0));
        HttpRequestHandler::service(request, response);
    }else if(request->path() == "/twitch_oauth" || request->path() == "/transmit_hash"){
        MyRequestHandler *controller = new MyRequestHandler(_enSettings, connection());
        controller->service(request, response);
    }else{
        _staticController->service(request, response);
    }

}
