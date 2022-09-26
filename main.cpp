#include "mainwindow.h"
#include <QApplication>
#include "gdatafactory.h"
#include <QDir>
#include "QsLog.h"
#include "QsLogDest.h"
#include <QFile>
#include <QTextCodec>
using namespace QsLogging;
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString qssFile = ":/icon/darkgray.qss";
    QFile file(qssFile);

    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);

    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath("log.txt"));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(512*1024), MaxOldLogCount(5)));
    logger.addDestination(fileDestination);

    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);

    DestinationPtr controlDestination(DestinationFactory::MakeFunctorDestination(GDataFactory::get_logs_widget(),SLOT(slot_rev_logs(const QString))));
    logger.addDestination(controlDestination);

    QLOG_INFO()<<u8"QsLog init SUCCESS";
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

    if(GDataFactory::get_pgsql()->ConnectDataBase())
        QLOG_INFO()<<"connect to db success!";
    else {
        QLOG_FATAL()<<"connect to db failed!";
        QApplication::exit(1);
    }
    GDataFactory::get_factory()->connect_common_signal_slot();

    QString binDir=QApplication::applicationDirPath();
    QString fileName("config.json");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using json config file"<<fileName;
            GDataFactory::get_factory()->load_json_config_1(QByteArray(fileName.toLatin1()).data());
        }
        else
        {
            QLOG_WARN()<<"Json config file not exist"<<fileName;
        }
    }

    if(GDataFactory::get_welcome_wgt()->exec() == QDialog::Accepted )
    {
        GDataFactory::get_factory()->init_http_service();
        GDataFactory::get_main_window()->show();
    }
    else
        QApplication::exit(1);
    return a.exec();
}
