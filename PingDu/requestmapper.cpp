#include "requestmapper.h"
#include "gdatafactory.h"
#include "fileuploadcontroller.h"
#include "datasliceuploadcontroller.h"
#include "filedownloadcontroller.h"
#include "imageuploadcontroller.h"
RequestMapper::RequestMapper(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    if (path=="/") {
        QueryHttpController().service(request, response);
    }
    else if (path=="/upload_slice") {
        DataSliceUploadController().service(request,response);
    }
    else if (path=="/cookie") {
        ;
    }
    else if(path == "/upload_img")
    {
        ImageUploadController().service(request,response);
    }
    else if(path=="/upload")
    {
        FileUploadController().service(request,response);
    }
    else if(path == "/download")
    {
        FileDownloadController().service(request,response);
    }
    else if (path.startsWith("/files")) {
        if(GDataFactory::get_static_file_controller()!=nullptr)
            GDataFactory::get_static_file_controller()->service(request,response);
        else {
            response.setStatus(404," static file controller is Not found");
            response.write("The URL is wrong, no such document.");
        }
    }
    else {
        response.setStatus(404,"Not found");
        response.write("The URL is wrong, no such document.");
    }

    qDebug("RequestMapper: finished request");
}
