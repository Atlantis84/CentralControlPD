#-------------------------------------------------
#
# Project created by QtCreator 2021-10-25T18:26:45
#
#-------------------------------------------------

QT += webenginewidgets core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CentralControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

SOURCES += \
        PingDu/boardstylewidget_pd.cpp \
        PingDu/datasliceuploadcontroller.cpp \
        PingDu/dbprocessobj_pd.cpp \
        PingDu/elecequiptestplanwgt_pd.cpp \
        PingDu/filedownloadcontroller.cpp \
        PingDu/fileuploadcontroller.cpp \
        PingDu/httppdmsg.cpp \
        PingDu/imageuploadcontroller.cpp \
        PingDu/requestmapper.cpp \
        PingDu/stationmonitorwgt_pd.cpp \
        PingDu/stationtabwgt_pd.cpp \
        PingDu/usertabwgt_pd.cpp \
        TV/boardstylewidget_tv.cpp \
        boardstylewidget.cpp \
        centralwidget.cpp \
        circlewidget.cpp \
        datacommserial.cpp \
        dragtreewidget.cpp \
        gdatafactory.cpp \
        historydisplaywgt.cpp \
        httpaoimsg.cpp \
        httpcpps/httpconnectionhandler.cpp \
        httpcpps/httpconnectionhandlerpool.cpp \
        httpcpps/httpcookie.cpp \
        httpcpps/httpglobal.cpp \
        httpcpps/httplistener.cpp \
        httpcpps/httprequest.cpp \
        httpcpps/httprequesthandler.cpp \
        httpcpps/httpresponse.cpp \
        httpcpps/httpsession.cpp \
        httpcpps/httpsessionstore.cpp \
        httpcpps/staticfilecontroller.cpp \
        httpcpps/template.cpp \
        httpcpps/templatecache.cpp \
        httpcpps/templateloader.cpp \
        httpmsgprocess.cpp \
        httpregistermsg.cpp \
        httptconmsg.cpp \
        httptvmsg.cpp \
        httpvisionmsg.cpp \
        imessagebox.cpp \
        leftwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        mesprocess.cpp \
        onebuttonchangewidget.cpp \
        qslogwidget.cpp \
        queryhttpcontroller.cpp \
        rightheadwidget.cpp \
        rightlabel.cpp \
        rightmainwidget.cpp \
        rightmainwindow.cpp \
        rightquerywidget.cpp \
        testplanwgt.cpp \
        testwidget.cpp \
        title/TitleBar.cpp \
        title/titlewidget.cpp \
        tvplanwgt.cpp \
        welcomewidget.cpp

HEADERS += \
        PingDu/boardstylewidget_pd.h \
        PingDu/datasliceuploadcontroller.h \
        PingDu/dbprocessobj_pd.h \
        PingDu/elecequiptestplanwgt_pd.h \
        PingDu/filedownloadcontroller.h \
        PingDu/fileuploadcontroller.h \
        PingDu/httppdmsg.h \
        PingDu/imageuploadcontroller.h \
        PingDu/requestmapper.h \
        PingDu/stationmonitorwgt_pd.h \
        PingDu/stationtabwgt_pd.h \
        PingDu/usertabwgt_pd.h \
        TV/1.h \
        TV/boardstylewidget_tv.h \
        boardstylewidget.h \
        centralwidget.h \
        circlewidget.h \
        datacommserial.h \
        dragtreewidget.h \
        gdatafactory.h \
        historydisplaywgt.h \
        httpaoimsg.h \
        httpheaders/httpconnectionhandler.h \
        httpheaders/httpconnectionhandlerpool.h \
        httpheaders/httpcookie.h \
        httpheaders/httpglobal.h \
        httpheaders/httplistener.h \
        httpheaders/httprequest.h \
        httpheaders/httprequesthandler.h \
        httpheaders/httpresponse.h \
        httpheaders/httpsession.h \
        httpheaders/httpsessionstore.h \
        httpheaders/staticfilecontroller.h \
        httpheaders/template.h \
        httpheaders/templatecache.h \
        httpheaders/templateglobal.h \
        httpheaders/templateloader.h \
        httpmsgprocess.h \
        httpregistermsg.h \
        httptconmsg.h \
        httptvmsg.h \
        httpvisionmsg.h \
        imessagebox.h \
        leftwidget.h \
        mainwindow.h \
        mesprocess.h \
        onebuttonchangewidget.h \
        qslogwidget.h \
        queryhttpcontroller.h \
        rightheadwidget.h \
        rightlabel.h \
        rightmainwidget.h \
        rightmainwindow.h \
        rightquerywidget.h \
        testplanwgt.h \
        testwidget.h \
        title/TitleBar.h \
        title/titlewidget.h \
        tvplanwgt.h \
        welcomewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += .\title
INCLUDEPATH += .\httpheaders
INCLUDEPATH += .\include
INCLUDEPATH += .\PingDu
INCLUDEPATH += .\TV

INCLUDEPATH += ..\QsLog-master
INCLUDEPATH += ..\DMDBAccess


DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
DEFINES += HTTP_ONLY

LIBS += -L$$PWD\lib\x64\ -lQsLog2
LIBS += -L$$PWD\lib\x64\ -lDMDBAccess

RESOURCES += \
    rc.qrc
