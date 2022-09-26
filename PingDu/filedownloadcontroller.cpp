#include "filedownloadcontroller.h"
#include "QsLog.h"
#include "gdatafactory.h"
FileDownloadController::FileDownloadController()
{

}

void FileDownloadController::service(HttpRequest &request, HttpResponse &response)
{
    QList<QByteArray> tmpdata = GDataFactory::get_DB_process_pd()->get_download_file_from_db(QString(request.getHeader("download_file")));
    if(tmpdata.size() != 0)
    {
        QByteArray responseData;
        for (int var = 0; var < tmpdata.size(); ++var) {
            responseData.append(tmpdata[var]);
        }
        response.setHeader("response_status","SUCCESS_PD");
        response.write(responseData,true);
    }
    else {
        response.setHeader("response_status","FAILED_PD");
        response.write("",true);
    }
}
