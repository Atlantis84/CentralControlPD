#ifndef FILEDOWNLOADCONTROLLER_H
#define FILEDOWNLOADCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
using namespace stefanfrings;

class FileDownloadController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FileDownloadController)
public:
    FileDownloadController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // FILEDOWNLOADCONTROLLER_H
