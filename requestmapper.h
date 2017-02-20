#include "httpserver.h"
#include "httprequesthandler.h"
#include "httpresponse.h"
#include "httprequest.h"
#include "myrequesthandler.h"

#include <httpsettings.h>
#include <staticfilecontroller.h>
using namespace HobrasoftHttpd;
class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
  public:
    void service(HttpRequest *request, HttpResponse *response);
    RequestMapper(EncryptedSettings *enSettings, Settings *settings, HttpConnection *connection);
private:
    StaticFileController *_staticController;
    EncryptedSettings *_enSettings;
    Settings *_settings;
};
class MyHttpd : public HttpServer {
    Q_OBJECT
  public:
    // The method returns pointer to new instance of your own request mapper
    // The request mapper maps requests like "/my-function" to call your own classes
    HttpRequestHandler *requestHandler(HttpConnection *connection);
    MyHttpd(EncryptedSettings *enSettings, Settings *settings, const HttpSettings *httpsettings, QObject *parent);
  private:
    EncryptedSettings *_enSettings;
    Settings *_settings;
};
