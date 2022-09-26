/**
  @file
  @author Stefan Frings
*/

#include "fileuploadcontroller.h"
#include "gdatafactory.h"
#include <QMutex>
#include <QMutexLocker>
static QMutex mutex;//for operation of QFile in multi-thread
FileUploadController::FileUploadController()
{
}
void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{
    mutex.lock();
    QTemporaryFile* file=request.getUploadedFile("upload_file");
    QByteArray aa = request.getParameter("upload_file");
    if ((file) && (aa.size() != 0))
    {
        GDataFactory::get_DB_process_pd()->delete_file_in_db(QString::fromLocal8Bit(aa));
        while (!file->atEnd() && !file->error())
        {
            QByteArray buffer=file->read(65536);
            GDataFactory::get_DB_process_pd()->save_pd_config_file_data(QString::fromLocal8Bit(aa),buffer);
        }
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

