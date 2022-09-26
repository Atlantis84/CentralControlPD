#include "httppdmsg.h"
#include "gdatafactory.h"
HttpPdMsg::HttpPdMsg()
{

}

void HttpPdMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));

    if(instructionValue.toString() == "PD_TEST")
    {
        QList<QString> lstAll = GDataFactory::get_factory()->get_change_production_machine_name();
        if(lstAll.count() == 0)
        {
            QLOG_WARN()<<"there is no change info in the change list!";
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("reType","PD_TEST");
            QJsonValue tmpValue = httpJsonObj.value("reLineName");
            reJson.insert("reLineName",tmpValue.toString());
            tmpValue = httpJsonObj.value("reStationName");
            reJson.insert("reStationName",tmpValue.toString());
            reJson.insert("reProjIDFileName","");
            reJson.insert("reConfFileName","");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("reType","PD_TEST");
            QJsonValue tmpValue = httpJsonObj.value(QStringLiteral("reLineName"));
            reJson.insert("reLineName",tmpValue.toString());
            tmpValue = httpJsonObj.value(QStringLiteral("reStationName"));
            reJson.insert("reStationName",tmpValue.toString());
            reJson.insert("reProjIDFileName",GDataFactory::get_factory()->get_config_para("PROJECT_D_FILE"));
            reJson.insert("reConfFileName",GDataFactory::get_factory()->get_config_para("CONFIG_FILE"));
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
    }
    else if(instructionValue.toString() == "PD_RESULT")
    {
        QLOG_INFO()<<httpJsonObj.value(QStringLiteral("reStationName")).toString()+" change production SUCCESS";
        GDataFactory::get_factory()->del_change_production_machine_name(httpJsonObj.value(QStringLiteral("reStationName")).toString());
        QJsonObject reJson;
        reJson.insert("code","OK");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
    else {
        QLOG_WARN()<<"PingDu Elec Equip request msg data exist ERROR!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("reType","PD_TEST");
        QJsonValue tmpValue = httpJsonObj.value(QStringLiteral("reLineName"));
        reJson.insert("reLineName",tmpValue.toString());
        tmpValue = httpJsonObj.value(QStringLiteral("reStationName"));
        reJson.insert("reStationName",tmpValue.toString());
        reJson.insert("reProjIDFileName","");
        reJson.insert("reConfFileName","");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
