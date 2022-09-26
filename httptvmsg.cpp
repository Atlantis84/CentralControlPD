#include "httptvmsg.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QJsonDocument>
#include <QJsonArray>
HttpTVMsg::HttpTVMsg()
{

}

void HttpTVMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
//    if(GDataFactory::get_factory()->get_tv_change_sign() == false)
//    {
//        GDataFactory::get_factory()->set_tv_change_sign(true);
//        QJsonObject root;
//        root.insert("code","OK");
//        root.insert("requestType","TCON_TEST");
//        QJsonArray  ats_value;
//        QJsonObject ats_models;
//        ats_models.insert("areaName","T104");
//        ats_models.insert("machineName",u8"机台4");
//        ats_models.insert("boardStyle",u8"11133");
//        ats_models.insert("machineStyleName",u8"");
//        ats_models.insert("halfMaterialNumber",u8"123456");
//        ats_value.append(ats_models);
//        root.insert("data",ats_value);
//        QJsonDocument doc_data(root);
//        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
//        response.write(response_data,true);
//    }
//    return;

    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    if(instructionValue.toString() == "TV_TEST")
    {
        QLOG_INFO()<<"rev TV test request MSG!";
//        QList<QList<QString>> lstAll = GDataFactory::get_factory()->get_all_machine_change_info();
        QList<QString> lstAll = GDataFactory::get_tv_plan_wgt()->get_change_product_info();
        if(lstAll.count() == 0)
        {
            QLOG_WARN()<<"there is no change info in the DB!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            if(lstAll.size() == 5)
            {
                QJsonObject root;
                root.insert("code","OK");
                root.insert("requestType","TV_TEST");
                QJsonArray  ats_value;
//                for (int i=0;i<lstAll.size();i++)
//                {
                QJsonObject ats_models;
                ats_models.insert("areaName",lstAll[3]);//T104
                ats_models.insert("machineName",lstAll[4]);//机台1
                ats_models.insert("boardStyle",lstAll[0]);
                ats_models.insert("machineStyleBom",lstAll[1]);//65E52G(DS)(BOM4)
                ats_models.insert("componentNum",lstAll[2]);//286050
                ats_value.append(ats_models);
//                }
                root.insert("data",ats_value);
                QJsonDocument doc_data(root);
                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                response.write(response_data,true);
                qDebug()<<response_data;
            }
            else {
                QLOG_WARN()<<"the tv change info is not SANE!";
                QJsonObject reJson;
                reJson.insert("code","ERROR");
                reJson.insert("data",u8"");
                QJsonDocument doc_data(reJson);
                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                response.write(response_data,true);
            }
        }
    }
    else if(instructionValue.toString() == "TV_RESULT")
    {
        QLOG_WARN()<<"rev TV change production result!";
        QString machineName = httpJsonObj.value(QStringLiteral("machineName")).toString();
        if(machineName.trimmed() != "")
        {
            QLOG_INFO()<<"del "<<machineName<<"from the change production list";
            GDataFactory::get_factory()->del_change_production_machine_name(machineName);
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else {
            QLOG_WARN()<<"tv change production result exist ERROR!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
    }
    else
    {
        QLOG_WARN()<<"tv request msg data exist ERROR!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("data",u8"");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
