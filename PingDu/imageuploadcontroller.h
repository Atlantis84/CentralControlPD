/**
  @file
  @author Stefan Frings
*/

#ifndef ImageUploadController_H
#define ImageUploadController_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller displays a HTML form for file upload and recieved the file.
*/


class ImageUploadController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(ImageUploadController)
public:

    /** Constructor */
    ImageUploadController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // ImageUploadController_H
