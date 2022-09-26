#include "httpaoimsg.h"
#include "QsLog.h"
#include "gdatafactory.h"
#include <QNetworkReply>
#include <QEventLoop>

HttpAOIMsg::HttpAOIMsg()
{

}

void HttpAOIMsg::process_http_msg(QJsonObject httpJsonObj,HttpResponse& response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    QLOG_INFO()<<"rev msg from AOI!";
    //just for test,delete the code later
    QJsonObject tmpReJson;
    if(instructionValue.toString() == "AOI_INFO")
    {
        QString manualBdType,manualHalfMaterialNum;
        manualBdType = "";
        manualHalfMaterialNum = "";
        GDataFactory::get_factory()->get_A_info(httpJsonObj.value(QStringLiteral("productSN")).toString(),manualBdType,manualHalfMaterialNum);
        if((manualBdType != "")&&(manualHalfMaterialNum != ""))
        {
            tmpReJson.insert("code","OK");
            tmpReJson.insert("boardType",manualBdType);
            tmpReJson.insert("boardName","A");
            tmpReJson.insert("boardId",manualHalfMaterialNum);
            QJsonDocument tmp_doc_data(tmpReJson);
            QByteArray tmp_response_data = tmp_doc_data.toJson(QJsonDocument::Compact);
            response.write(tmp_response_data,true);
        }
        else {
            QString tmpBoardStyle="";
            for (int i=0;i<5;i++)
            {
                tmpBoardStyle.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
            }
            tmpReJson.insert("code","OK");
            tmpReJson.insert("boardType",tmpBoardStyle);
            tmpReJson.insert("boardName","B");
            tmpReJson.insert("boardId",GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));
            QJsonDocument tmp_doc_data(tmpReJson);
            QByteArray tmp_response_data = tmp_doc_data.toJson(QJsonDocument::Compact);
            response.write(tmp_response_data,true);
        }
    }
    return;
    //just for test,delete the code later

    if(instructionValue.toString() == "AOI_INFO")
    {
        QString productsn = httpJsonObj.value(QStringLiteral("productSN")).toString();
        QString tmpurl;
        tmpurl = "http://";
        tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
        tmpurl.append(":");
        tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
        tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
        tmpurl.append(productsn);

        QNetworkReply* reply;
        QEventLoop eventloop;
        QNetworkRequest* request = new QNetworkRequest();
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

                                QString aType = GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE");
                                QString bType = GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE");
                                QString formalBoardType="";
                                if(icon["tpS_TEMPLATE"].toString() == aType)
                                    formalBoardType = "A";
                                else if(icon["tpS_TEMPLATE"].toString() == bType)
                                    formalBoardType = "B";
                                else
                                    formalBoardType = "";

                                if( formalBoardType!="")
                                {
                                    reJson.insert("code","OK");
                                    reJson.insert("boardType",icon["tpS_TEMPLATE"].toString());
                                    reJson.insert("boardName",formalBoardType);
                                    reJson.insert("boardId",icon["modeL_CODE"].toString());
                                    QJsonDocument doc_data(reJson);
                                    QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                                    response.write(response_data,true);
                                }
                                else
                                {
                                    QLOG_WARN()<<"get A or B from gdatafactory exist ERROR!";
                                    QJsonObject reJsonError;
                                    reJsonError.insert("code","ERROR");
                                    reJsonError.insert("data",u8"");
                                    QJsonDocument doc_data(reJsonError);
                                    QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
                                    response.write(response_data,true);
                                    return;
                                }
                            }
                        }
                    }
                    else
                    {
                        QLOG_WARN()<<"the data of MES exist ERROR!";
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
    else if(instructionValue.toString() == "AOI_INIT")
    {
        QLOG_WARN()<<"reply AOI init msg!";
        QJsonObject reJson,arrayObject1,arrayObject2;
        QJsonArray  arrayValue;

        reJson.insert("code","OK");
        QString aBoardType = GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE");
        QString aHalfMaterialNumber = GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A");
        QString bBoardType = GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE");
        QString bHalfMaterialNumber = GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B");
//        if(lstA.length() == 5)
        if(aBoardType != "")
        {
            arrayObject1.insert("boardType",aBoardType);//add board type info into the AOI reply msg
            arrayObject1.insert("boardName","A");
            arrayObject1.insert("boardId",aHalfMaterialNumber);
            arrayValue.append(arrayObject1);
        }
        else
        {
            QLOG_WARN()<<"product A change info-list exist ERROR!";
            QJsonObject reJsonError;
            reJsonError.insert("code","ERROR");
            reJsonError.insert("data",u8"");
            QJsonDocument doc_data(reJsonError);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            return;
        }

//        if(lstB.length() == 5)
        if(bBoardType != "")
        {
            arrayObject2.insert("boardType",bBoardType);//add board type info into the AOI reply msg
            arrayObject2.insert("boardName","B");
            arrayObject2.insert("boardId",bHalfMaterialNumber);
            arrayValue.append(arrayObject2);
        }
        else
        {
            QLOG_WARN()<<"product B change info-list exist ERROR!";
            QJsonObject reJsonError;
            reJsonError.insert("code","ERROR");
            reJsonError.insert("data",u8"");
            QJsonDocument doc_data(reJsonError);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            return;
        }
        reJson.insert("data",arrayValue);
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }
    else
    {
        QLOG_WARN()<<"AOI request MSG exist ERROR!";
        QJsonObject reJsonError;
        reJsonError.insert("code","ERROR");
        reJsonError.insert("data",u8"");
        QJsonDocument doc_data(reJsonError);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }
}
