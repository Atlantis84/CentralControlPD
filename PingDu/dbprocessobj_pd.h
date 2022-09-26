#ifndef DBPROCESSOBJ_PD_H
#define DBPROCESSOBJ_PD_H

#include <QObject>
#include <QMutex>
class DBProcessObj_pd : public QObject
{
    Q_OBJECT
public:
    explicit DBProcessObj_pd(QObject *parent = nullptr);

    void save_pd_config_file_data(QString filename,QByteArray dataslice);
    bool delete_file_in_db(QString filename);
    QList<QByteArray> get_download_file_from_db(QString filename);
    QList<QString> get_pd_change_file_name(QString filename_part);
private:
    QMutex m_mutex;

signals:

public slots:
};

#endif // DBPROCESSOBJ_PD_H
