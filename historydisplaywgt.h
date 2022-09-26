#ifndef HISTORYDISPLAYWGT_H
#define HISTORYDISPLAYWGT_H

#include <QWidget>
class QTableWidget;
class HistoryDisplayWgt : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryDisplayWgt(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
public:


public slots:
};

#endif // HISTORYDISPLAYWGT_H
