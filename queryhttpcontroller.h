#ifndef QUERYHTTPCONTROLLER_H
#define QUERYHTTPCONTROLLER_H
#include "httprequesthandler.h"
using namespace stefanfrings;
class QueryHttpController : public HttpRequestHandler
{
    Q_OBJECT
public:
    QueryHttpController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // TCONHTTPCONTROLLER_H
