#ifndef STATIONTABWGT_H
#define STATIONTABWGT_H

#include <QWidget>
#include <QTableWidget>
class QCheckBox;
class QComboBox;
class QLineEdit;
class StationTabWgt : public QWidget
{
    Q_OBJECT
public:
    explicit StationTabWgt(QWidget *parent = nullptr);

private:
    QWidget* createTableWidget();
    void set_table_data();
    void showEvent(QShowEvent *event);
    QTableWidget* tableWidget;
    QCheckBox* checkStationName;
    QCheckBox* checkProduceCapacity;
    QCheckBox* checkProduceCadence;
    QCheckBox* checkDate;
    QCheckBox* checkTime;
    QComboBox* cmbUserPermsn;
    QLineEdit* lineEditUserName;
    QLineEdit* lineEditPassword;

signals:

public slots:
    void slot_edit_user();
    void slot_delete_user();
    void slot_add_user();
    void slot_item_selected(QTableWidgetItem* it);
};

#endif // STATIONTABWGT_H
