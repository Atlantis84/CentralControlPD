#include "queryhttpcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include "gdatafactory.h"
#include "QsLog.h"
#include <QJsonParseError>
QueryHttpController::QueryHttpController(QObject* parent)
    : HttpRequestHandler(parent)
{

}

void QueryHttpController::service(HttpRequest &request, HttpResponse &response)
{
    QJsonParseError jError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(request.getBody().data(),&jError);
    QJsonObject jsonObject = jsonDocument.object();
    //rev and process data from registerstation\AOI\visuallead-lilei20211112
    if(jsonObject.contains("reType"))
    {
        QJsonValue instructionValue = jsonObject.value("reType");
        if(GDataFactory::get_factory()->get_http_process_obj(instructionValue.toString()) == nullptr)
        {
            QLOG_WARN()<<"get http obj failed!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data","");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            GDataFactory::get_factory()->
                    get_http_process_obj(instructionValue.toString())->
                    process_http_msg(jsonObject,response);
        }
    }
    else
    {
        QLOG_WARN()<<"the client request exist ERROR!";
        QLOG_WARN()<<request.getBody().data();
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("data","");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
