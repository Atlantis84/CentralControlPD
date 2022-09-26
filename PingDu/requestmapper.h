#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H
#include "httprequesthandler.h"
#include "queryhttpcontroller.h"
using namespace stefanfrings;
class RequestMapper : public HttpRequestHandler
{
    Q_OBJECT
public:
    RequestMapper(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
private:
};

#endif // REQUESTMAPPER_H
