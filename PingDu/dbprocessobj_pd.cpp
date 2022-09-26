#include "dbprocessobj_pd.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDateTime>
#include <QMutexLocker>
DBProcessObj_pd::DBProcessObj_pd(QObject *parent) : QObject(parent)
{

}

void DBProcessObj_pd::save_pd_config_file_data(QString filename, QByteArray dataslice)
{
    QMutexLocker lock(&m_mutex);
    QString MD5 = "";
    QByteArray tmpByteArray;
    tmpByteArray = QCryptographicHash::hash(dataslice,QCryptographicHash::Md5);
    MD5.append(tmpByteArray.toHex());

    int maxID = GDataFactory::get_factory()->get_biggest_id(constPDConfigFileTable);
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
    QString strSql = QString("insert into %1 values(%2,'%3','%4','%5')").
                arg(constPDConfigFileTable).
                arg(maxID).
                arg(filename).
                arg(MD5).
                arg(current_date);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
    {
//        QLOG_INFO()<<"insert constPDConfigFileTable success!";

        strSql = QString("select * from %1 where file_name='%2' and md5_value='%3'").
                arg(constPDConfigFileDataTable).arg(filename).arg(MD5);
        QSqlQuery queryResult;
        if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
        {
//            QLOG_INFO()<<"query constPDConfigFileDataTable success!";
            if(queryResult.size()!=0)
            {
                ;//
            }
            else {
                strSql = QString("insert into %1 values('%2','%3','%4','%5')").
                        arg(constPDConfigFileDataTable).
                        arg(filename).
                        arg(QString(dataslice.toHex())).
                        arg(MD5).
                        arg(current_date);
                QString strErrorMsg;
                if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
                {
                    ;
                }
                else {
                    ;
                    QLOG_ERROR()<<"insert into constPDConfigFileDataTable ERROR";
                }
            }
        }
        else
            QLOG_WARN()<<"query constPDConfigFileDataTable failed!";
    }
    else
        QLOG_WARN()<<"insert file info failed!";
}

bool DBProcessObj_pd::delete_file_in_db(QString filename)
{
    QMutexLocker lock(&m_mutex);
    QString strSql = QString("delete from public.%1 where \"file_name\"=\'%2\'").arg(constPDConfigFileTable).arg(filename);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QList<QByteArray> DBProcessObj_pd::get_download_file_from_db(QString filename)
{
    QMutexLocker lock(&m_mutex);
    QList<QByteArray> lstFile;
    QString strSql = QString("select * from %1 where file_name='%2'").
            arg(constPDConfigFileTable).
            arg(filename);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            QString md5 = queryResult.value(2).toString();
            strSql = QString("select * from %1 where file_name='%2' and md5_value = '%3'").
                    arg(constPDConfigFileDataTable).
                    arg(filename).
                    arg(md5);
            QSqlQuery queryResult1;
            if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult1))
            {
                while(queryResult1.next())
                {
                    lstFile.push_back(QByteArray::fromHex(queryResult1.value(1).toString().toLatin1()));
                }
            }
            else {
                QLOG_WARN()<<"query constPDConfigFileDataTable failed";
            }
        }
    }
    else {
        QLOG_WARN()<<"query constPDConfigFileTable failed";
    }
    return lstFile;
}

QList<QString> DBProcessObj_pd::get_pd_change_file_name(QString filename_part)
{
    QMutexLocker lock(&m_mutex);
    QList<QString> tmpLst;
    QString strSql = QString("select * from %1 where file_name ~*'%2'").
            arg(constPDConfigFileTable).
            arg(filename_part);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            tmpLst.push_back(queryResult.value(1).toString());
        }
    }
    return tmpLst;
}
