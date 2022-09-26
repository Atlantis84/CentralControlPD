/**
  @file
  @author Stefan Frings
*/

#include "ImageUploadController.h"
#include "gdatafactory.h"
#include <QMutex>
#include <QMutexLocker>
static QMutex mutex;//for operation of QFile in multi-thread
ImageUploadController::ImageUploadController()
{
}
void ImageUploadController::service(HttpRequest& request, HttpResponse& response)
{
    mutex.lock();
    QTemporaryFile* file=request.getUploadedFile("upload_file");
    QByteArray aa = request.getParameter("upload_file");
    if ((file) && (aa.size() != 0))
    {
        QFile writeFile("F:\\test.png");
        writeFile.open(QIODevice::ReadWrite);
        writeFile.write(file->readAll());
        writeFile.close();
        response.write("SUCCESS_PD");
        QLOG_WARN()<<"File Upload Controller rev the file uploaded SUCCESS";
        mutex.unlock();
    }
    else
    {
        response.write("FAILED_PD");
        QLOG_WARN()<<"File Upload Controller rev the file uploaded FAILED";
        mutex.unlock();
    }
}

