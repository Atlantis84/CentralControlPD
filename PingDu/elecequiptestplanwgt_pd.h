#ifndef ELECEQUIPTESTPLANWGT_H
#define ELECEQUIPTESTPLANWGT_H

#include <QWidget>
#include "boardstylewidget_pd.h"
#include "dragtreewidget.h"
#include <QComboBox>
class ElecEquipTestPlanWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ElecEquipTestPlanWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    int m_timer_ID;
    int m_time_out;

    BoardStyleWidgetPd* bsWgtPd;
    DragTreeWidget* dragTreeWgt;
    QComboBox* combProductStyle;
    bool check_product_exist();

signals:
    void signal_change_btn_icon();
    void signal_save_plan();
public slots:
    void slot_save_test_plan();
    void slot_kill_timeout_timer();
};

#endif // ELECEQUIPTESTPLANWGT_H
