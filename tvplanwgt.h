#ifndef TVPLANWGT_H
#define TVPLANWGT_H

#include <QWidget>
#include <QMutex>
#include <QMutexLocker>
#include "dragtreewidget.h"
class QLineEdit;
class QComboBox;
class TvPlanWgt : public QWidget
{
    Q_OBJECT
public:
    explicit TvPlanWgt(QWidget *parent = nullptr);
    void set_serial_number(QString serialnum);
    void set_change_product_info(QList<QString> lstinfo)
    {
        QMutexLocker lock(&m_mutex);
        m_pListChangeInfo = lstinfo;
    }

    QList<QString> get_change_product_info()
    {
        QMutexLocker lock(&m_mutex);
        return m_pListChangeInfo;
    }
private:
    QMutex m_mutex;
    QLineEdit* lineProductSN;
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    QList<QString> m_pListChangeInfo;
    QComboBox* cmbLineName;
    QComboBox* cmbStationName;
    DragTreeWidget* dragTreeWgt;

signals:
    void signal_change_btn_icon();
public slots:
    void slot_save_test_plan();
};

#endif // TVPLANWGT_H
