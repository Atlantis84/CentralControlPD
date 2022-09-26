#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
public:

signals:

public slots:
};

#endif // CENTRALWIDGET_H
