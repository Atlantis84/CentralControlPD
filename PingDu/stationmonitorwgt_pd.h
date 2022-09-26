#ifndef STATIONMONITORWGT_H
#define STATIONMONITORWGT_H
#include <QWidget>
#include <QWebEngineView>
#include <QtWebEngine/qtwebengineglobal.h>

class StationMonitorWgt : public QWidget
{
    Q_OBJECT
public:
    explicit StationMonitorWgt(QWidget *parent = nullptr);

private:

    void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // STATIONMONITORWGT_H
