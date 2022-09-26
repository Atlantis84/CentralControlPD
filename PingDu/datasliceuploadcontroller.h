#ifndef DATASLICEUPLOADCONTROLLER_H
#define DATASLICEUPLOADCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"
using namespace stefanfrings;
class DataSliceUploadController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(DataSliceUploadController)
public:
    DataSliceUploadController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // DATASLICEUPLOADCONTROLLER_H
