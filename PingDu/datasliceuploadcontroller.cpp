#include "datasliceuploadcontroller.h"
#include "gdatafactory.h"
DataSliceUploadController::DataSliceUploadController()
{

}

void DataSliceUploadController::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray bb = request.getBody().data();
    GDataFactory::get_DB_process_pd()->save_pd_config_file_data(QString::fromLocal8Bit("json.json"),bb);
}
