#include "httpvisionmsg.h"
#include "QsLog.h"
#include <QJsonDocument>
#include "gdatafactory.h"
#include <QEventLoop>
#include <QNetworkReply>
#include "imessagebox.h"
#include <QJsonArray>
#include <QSslKey>
#include <QSslConfiguration>
HttpVisionMsg::HttpVisionMsg()
{

}

void HttpVisionMsg::process_http_msg(QJsonObject httpJsonObj,HttpResponse& response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    QLOG_INFO()<<"rev msg from VISION!";
    if((instructionValue.isString()) && (instructionValue.toString() != ""))
    {
        //just for test,delete later
        QJsonObject reJson1;
        QString manualBdType,manualHalfMaterialNum;
        manualBdType = "";
        manualHalfMaterialNum = "";
        GDataFactory::get_factory()->get_A_info(httpJsonObj.value(QStringLiteral("productSN")).toString(),manualBdType,manualHalfMaterialNum);
        if((manualBdType != "")&&(manualHalfMaterialNum != ""))
        {
            reJson1.insert("code","OK");
            reJson1.insert("projectID",GDataFactory::get_factory()->get_config_para("PROJECT_ID_A"));
            reJson1.insert("halfMaterialNumber",manualHalfMaterialNum);
            reJson1.insert("machineStyleName",GDataFactory::get_factory()->get_config_para("MACHINE_STYLE_NAME_A"));
            reJson1.insert("boardStyle",manualBdType);
            reJson1.insert("boardSize",GDataFactory::get_factory()->get_config_para("BOARD_SIZE_A"));
        }
        else
        {

            reJson1.insert("code","OK");
            reJson1.insert("projectID",GDataFactory::get_factory()->get_config_para("PROJECT_ID_B"));
            reJson1.insert("halfMaterialNumber",GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));
            reJson1.insert("machineStyleName",GDataFactory::get_factory()->get_config_para("MACHINE_STYLE_NAME_B"));
            QString tmpBoardStyle="";
            for (int i=0;i<5;i++)
            {
                tmpBoardStyle.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
            }
            reJson1.insert("boardStyle",tmpBoardStyle);
            reJson1.insert("boardSize",GDataFactory::get_factory()->get_config_para("BOARD_SIZE_B"));
        }
        QJsonDocument doc_data1(reJson1);
        QByteArray response_data1 = doc_data1.toJson(QJsonDocument::Compact);
        response.write(response_data1,true);
        QLOG_INFO()<<"reply VISION is normal!";
        return;
        //just for test,delete later

        QNetworkReply* reply;
        QEventLoop eventloop;
        QNetworkRequest* request = new QNetworkRequest();
        QString productsn = httpJsonObj.value(QStringLiteral("productSN")).toString();
        QString tmpurl;
        tmpurl = "http://";
        tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
        tmpurl.append(":");
        tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
        tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
        tmpurl.append(productsn);

        //just for test,delete the code later
//        QLOG_WARN()<<"MES status para data format is ABNORMAL!";
//        QJsonObject reJsonError;
//        reJsonError.insert("code","ERROR");
//        reJsonError.insert("data",u8"");
//        QJsonDocument doc_data(reJsonError);
//        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
//        response.write(response_data,true);
//        return;
        //just for test,delete the code later

        request->setUrl(QUrl(tmpurl));
        request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        request->setRawHeader("appId","CenterController");
        request->setRawHeader("publicKey",PUBLIC_KEY);

        QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager();
        reply = tmpaccessManager->get(*request);
        connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        //set get request time out
        QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
        eventloop.exec();

        if(reply->isFinished())
        {
            if (reply->error() == QNetworkReply::NoError){

                QByteArray responseByte = reply->readAll();
                QLOG_DEBUG()<<"the response of MES is:" << QString(responseByte);
                QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);

                QJsonObject jsonObject = jsonDocument.object();
                QString statusString = "";
                if(jsonObject.contains("status"))
                {
                    QJsonValue instructionValue = jsonObject.value(QStringLiteral("status"));
                    if(instructionValue.isString())
                        statusString = instructionValue.toString();
                    else
                    {
                        QLOG_WARN()<<"MES status para data format is ABNORMAL!";
                        QJsonObject reJsonError;
                        reJsonError.insert("code","ERROR");
                        reJsonError.insert("data",u8"");
                        QJsonDocument doc_data(reJsonError);
                        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                        response.write(response_data,true);
                        return;
                    }
                }

                if(statusString == "success")
                {
                    if(jsonObject.contains("dataInfo"))
                    {
                        QJsonValue arrayValue = jsonObject.value(QStringLiteral("dataInfo"));
                        if(arrayValue.isArray())
                        {
                            QJsonArray array = arrayValue.toArray();
                            for (int i=0;i<array.size();i++)
                            {
                                QJsonValue iconArray = array.at(i);
                                QJsonObject icon = iconArray.toObject();
                                QJsonObject reJson;
                                reJson.insert("code","OK");
                                reJson.insert("projectID",icon["projecT_ID"].toString());
                                reJson.insert("halfMaterialNumber",icon["modeL_CODE"].toString());
                                reJson.insert("machineStyleName",icon["mP_MODEL_CODE"].toString());
                                reJson.insert("boardStyle",icon["tpS_TEMPLATE"].toString());
                                reJson.insert("boardSize",icon["boarD_SIZE"].toString());
                                QJsonDocument doc_data(reJson);
                                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
//                                QLOG_INFO()<<"reple the VISION data is:"<<response_data;
                                response.write(response_data,true);
                            }
                        }
                    }
                }
                else
                {
                    QLOG_WARN()<<"MES status para is error!";
                    QJsonObject reJsonError;
                    reJsonError.insert("code","ERROR");
                    reJsonError.insert("data",u8"");
                    QJsonDocument doc_data(reJsonError);
                    QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                    response.write(response_data,true);
                    return;
                }
            }
            else
            {
                QLOG_WARN()<<"MES reply exist ERROR!";
                QJsonObject reJsonError;
                reJsonError.insert("code","ERROR");
                reJsonError.insert("data",u8"");
                QJsonDocument doc_data(reJsonError);
                QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                response.write(response_data,true);
                return;
            }
        }
        else
        {
            disconnect(reply,&QNetworkReply::finished,&eventloop,&QEventLoop::quit);
            reply->abort();

            QLOG_WARN()<<"MES reply TIMEOUT!";
            QJsonObject reJsonError;
            reJsonError.insert("code","ERROR");
            reJsonError.insert("data",u8"");
            QJsonDocument doc_data(reJsonError);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            return;
        }
        reply->deleteLater();
    }
    else
    {
        QLOG_WARN()<<"vision request msg data exist ERROR!";
        QJsonObject reJsonError;
        reJsonError.insert("code","ERROR");
        reJsonError.insert("data",u8"");

        QJsonDocument doc_data(reJsonError);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }
}
