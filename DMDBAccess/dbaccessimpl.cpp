#include "dbaccessimpl.h"
#include <QSqlError>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
static QMutex mutex;


void customMessageHandler(QtMsgType type,
                          const QMessageLogContext &context,
                          const QString &msg)
{
    //Q_UNUSED(context)
    QDateTime _datetime = QDateTime::currentDateTime();
    QString szDate = _datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");//"yyyy-MM-dd hh:mm:ss ddd"
    QString txt(szDate);

    switch (type)
    {
        case QtDebugMsg://调试信息提示
        {
            txt += QString(" [Debug] ");
            break;
        }
        case QtInfoMsg://信息输出
        {
            txt += QString(" [Info] ");
            break;
        }
        case QtWarningMsg://一般的warning提示
        {
            txt += QString(" [Warning] ");
            break;
        }
        case QtCriticalMsg://严重错误提示
        {
            txt += QString(" [Critical] ");
            break;
        }
        case QtFatalMsg://致命错误提示
        {
            txt += QString(" [Fatal] ");
            //abort();
            break;
        }
        default:
        {
            txt += QString(" [Trace] ");
            break;
        }
    }

    txt.append( QString(" %1").arg(context.file) );
    txt.append( QString("<%1>: ").arg(context.line) );
    txt.append(msg);

    mutex.lock();
    QFile file("db_log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << txt << "\r\n";
    file.close();
    mutex.unlock();
}

#define GBK(x)      QString::fromLocal8Bit(x)
#define GBK_LOG(x)  QString::fromLocal8Bit(x).toUtf8().data()

DBAccessImpl::DBAccessImpl()
{
    //注册MessageHandler
    qInstallMessageHandler(customMessageHandler);

    qDebug() << "this is QtDebugMsg";
    qInfo() << "this is QtInfoMsg";
    qWarning() << "this is QtWarningMsg";
    qCritical() << "this is QtCriticalMsg";

    qDebug() << GBK("测试qDebug");
    qInfo() << GBK("测试qInfo");
    qWarning() << GBK_LOG("测试qWarning");
    qCritical() << GBK_LOG("测试qCritical");
}

bool DBAccessImpl::ConnectDataBase()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE);
    m_pDatabase.setDatabaseName(DATABASENAME);
    m_pDatabase.setPort(PORTNUM);
    m_pDatabase.setHostName(HOSTNAME);
    m_pDatabase.setUserName(USERNAME);
    m_pDatabase.setPassword(PASSWORD);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::ConnectDataBaseMySQL()
{
    m_pDatabase = QSqlDatabase::addDatabase(DATABASESTYLE_MYSQL);
    m_pDatabase.setDatabaseName(DATABASENAME_MYSQL);
    m_pDatabase.setPort(PORTNUM_MYSQL);
    m_pDatabase.setHostName(HOSTNAME_MYSQL);
    m_pDatabase.setUserName(USERNAME_MYSQL);
    m_pDatabase.setPassword(PASSWORD_MYSQL);
    if(m_pDatabase.open())
        return true;
    else
        return false;
}

bool DBAccessImpl::PingReadDB()
{
    return 0;
}

QSqlDatabase &DBAccessImpl::GetReadDataBase()
{
    QSqlDatabase ss;
    return ss;
}

bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel &model)
{
    model.setQuery(strSql, m_pDatabase);
    if (model.lastError().isValid())
    {
//        QMessageBox::critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel& model) failed. "
//                                                     "query sql:%2. error info from database:%1")
//                             .arg(model.lastError().text())
//                             .arg(strSql));
        qWarning() << QString("GetQueryModel ERROR ").append(model.lastError().text()).append(strSql);
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryModel(const QString &strSql, QSqlQueryModel *model)
{
    model->setQuery(strSql, m_pDatabase);
    if (model->lastError().isValid())
    {
//        QMessageBox::critical(NULL, "ERROR", QString("GetQueryModel(const QString &strSql, QSqlQueryModel* model) failed. "
//                                                     "query sql:%2. error info from database:%1")
//                             .arg(model->lastError().text())
//                             .arg(strSql));
        qWarning() << QString("GetQueryModel ERROR ").append(model->lastError().text()).append(strSql);
        return false;
    }
    return true;
}

bool DBAccessImpl::GetQueryResult(const QString &strSql, QSqlQuery &sqlQuery)
{

    sqlQuery = QSqlQuery(m_pDatabase);
    qDebug()<<strSql;
    if(!sqlQuery.exec(strSql))
    {
//        QMessageBox::critical(NULL, "ERROR", QString("GetQueryResult(const QString& strSql, QSqlQuery& sqlQuery) failed. "
//                                                     "query sql:%2. error info from database:%1")
//                             .arg(sqlQuery.lastError().text())
//                              .arg(strSql));
        qWarning() << QString("GetQueryResult ERROR ").append(sqlQuery.lastError().text()).append(strSql);
        return false;
    }
    return true;
}

bool DBAccessImpl::ExecSql(QString &strsql, QString &strErrMsg)
{
    FormatSql(strsql);

    QSqlQuery sqlQuery = QSqlQuery(m_pDatabase);

    if(!sqlQuery.exec(strsql))
    {
//        QMessageBox::critical(NULL, "ERROR", QString("ExecSql(QString& strSql, QString& strErrMsg) failed. "
//                                                     "query sql:%2. error info from database:%1")
//                             .arg(sqlQuery.lastError().text())
//                              .arg(strsql));
        qWarning() << QString("ExecSql ERROR ").append(sqlQuery.lastError().text()).append(strsql);
        return false;
    }
    return true;

}

void DBAccessImpl::FormatSql(QString& strSql)
{
    strSql.trimmed();
    if(strSql[strSql.size()-1] != QChar(';'))
    {
        strSql.append(QString(";"));
    }
}
