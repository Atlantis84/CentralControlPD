#ifndef GDATAFACTORY_H
#define GDATAFACTORY_H

#include <QObject>
#include "mainwindow.h"
#include "welcomewidget.h"
#include "dmdbaccessinterface.h"
#include "centralwidget.h"
#include "leftwidget.h"
#include "rightheadwidget.h"
#include "rightmainwindow.h"
#include "rightmainwidget.h"
#include "rightquerywidget.h"
#include <QSerialPort>
#include "testwidget.h"
#include "datacommserial.h"
#include "httplistener.h"
#include "httprequesthandler.h"
#include "staticfilecontroller.h"
#include "mesprocess.h"
#include "testplanwgt.h"
#include "boardstylewidget.h"
#include "httpmsgprocess.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "qslogwidget.h"
#include "historydisplaywgt.h"
#include "QsLog.h"
#include "elecequiptestplanwgt_pd.h"
#include "stationtabwgt_pd.h"
#include "usertabwgt_pd.h"
#include "stationmonitorwgt_pd.h"
#include "staticfilecontroller.h"
#include "dbprocessobj_pd.h"
#include "tvplanwgt.h"
#include "boardstylewidget_pd.h"
#include "boardstylewidget_tv.h"
//for tcon
#define ADDRESS_D600 0x1770 //D6000
#define ADDRESS_D302 0x0BBA //D3002
#define WRITE_PLC 0x02
#define WRITE_VISION 0x01
//for tcon

//for elec equipment
//for elec equipment
using namespace stefanfrings;
#define PUBLIC_KEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqyHeiIAoTZ1I3Nb9KE8DoSMPWEMdA4ilQGh0yGdUzqvbU2XfpK5aNIen9aWqDUb7hV5rjSc6ab/sw5Zp2EJtGKwZB/JzVaLIwlx7Dvijx9FojiQ1Ca+7qJF74b9CnDkIWrIsKBiHvFQQfgxKUACfXxwsLirfE8pcE309wcXlldBcGFY/UgyYi6MJZcP7Es9a44Fb3cH8iluEBw3x85BvGhAsFMp/1cx4FcUflgSr/L7qWIJbH90yuSaQEFJd8xMb2sJViZrogknD4KQzMd7gmdygaDtyxujh517U/vpgNGbxgpaCLt4u8EZDyP+Pbyxu4TI1LZlv3RJry95M7dn2mQIDAQAB"
class IResponseValidate_F1 : public IResponseValidate
{
public:
    virtual bool validate(const QByteArray& response)
    {
        Q_UNUSED(response);
        bool bValid = true;
        return bValid;
    }
};

enum LineInfoValue{
    TCON_LINE=0,
    ELEC_EQUIP_LINE=1,
    TV_LINE=2
};

class GDataFactory : public QObject
{
    Q_OBJECT
public:
    GDataFactory();
    ~GDataFactory();
    void read_serial_number();
    void stop_serial_port();
    void connect_common_signal_slot();
    void load_json_config_1(char* filename);
    QString get_config_para(const QString keyname);    
    void set_config_para_1(const QString keyname, const QString keyvalue);
    void save_config_file();

    //**after we change the production,we put the machine stations that change production into the
    //list,when we receive change production completely msg from tcon,we remove the according
    //station name from the list
    void add_change_production_machine_name(QString machinename){
        m_pMachineNamesOfChangeProduction.push_back(machinename);
    }
    QList<QString> get_change_production_machine_name()
    {
        return m_pMachineNamesOfChangeProduction;
    }
    void del_change_production_machine_name(QString machinename);
    //**after we change the production,we put the machine stations that change production into the
    //list,when we receive change production completely msg from tcon,we remove the according
    //station name from the list

    //after we change the production,we send the change info(11111222222,the first 5 bits is board-style
    //the last 5 bits is half-material-number)to PLC or AOI
    void add_change_production_info_for_PLC_or_AOI(QString machinename,QString boardtype,QString halfmaterialnumber);
    QMap<QString,QString> get_PLC_or_AOI_change_info(QString keyvalue);
    //after we change the production,we send the change info(11111222222,the first 5 bits is board-style
    //the last 5 bits is half-material-number)to PLC or AOI

    QString bytes_to_str(QByteArray data);
    quint8 get_last_ip(const int id);
    void init_http_service();
    void clear_alter_production();
    QList<QList<QString>> get_all_machine_change_info();
    QSqlQuery get_query_result(QString strSql);
    void save_AB_change_product_info(QList<QString> changeinfo);
    void set_AB_change_product_info(QList<QString> boardinfo);
    QList<QString> get_A_change_product_info();
    QList<QString> get_B_change_product_info();

    int get_biggest_id(const QString& tablename);
    bool check_mac_exist(const QString& macaddr,QString* machinename);
    QString register_new_mac_machine(QList<QString> lstpara);
    QMap<QString,QList<QString>> get_all_machine_name_from_db();
    QString get_line_name_by_machine_name(QString machinename);

    QMutex m_mutex;
    QMutex m_mutex_PLC_AOI;
    bool truncate_change_info_table();

    //login for process password
    QString EncodeStr(QString password);
    QString HandleStr(int i);
    QString DecodeStr(QString s);

    //just for test
    void clear_A_sn()
    {
        m_pManualSN_A.clear();
    }
    void add_A_sn(int id,QString sn)
    {
        m_pManualSN_A.insert(id,sn);
    }

    void get_A_info(QString sn,QString& boardtype,QString& halfmaterialnum)
    {
        boardtype = "";
        halfmaterialnum = "";
        QMap<int,QString>::Iterator itr = m_pManualSN_A.begin();
        while (itr != m_pManualSN_A.end()) {
            if(itr.value() == sn)
            {
                QString tmpBoardStyle="";
                for (int i=0;i<5;i++)
                {
                    tmpBoardStyle.append(GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE")[i]);
                }
                boardtype = tmpBoardStyle;
                halfmaterialnum = GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A");
                break;
            }
            else
            {
                itr++;
                continue;
            }
        }

        QLOG_INFO()<<"the sn from VISION is:"<<sn;
        QLOG_INFO()<<"the current bdtype is:"<<boardtype;
        QLOG_INFO()<<"the current halfnum is:"<<halfmaterialnum;
    }

    void clear_B_sn()
    {
        m_pManualSN_B.clear();
    }

    void add_B_sn(int id,QString sn)
    {
        m_pManualSN_B.insert(id,sn);
    }

    void get_B_info(QString sn,QString& boardtype,QString& halfmaterialnum)
    {
        boardtype = "";
        halfmaterialnum = "";
        QMap<int,QString>::Iterator itr = m_pManualSN_B.begin();
        while (itr != m_pManualSN_B.end()) {
            if(itr.value() == sn)
            {
                QString tmpBoardStyle="";
                for (int i=0;i<5;i++)
                {
                    tmpBoardStyle.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
                }
                boardtype = tmpBoardStyle;
                halfmaterialnum = GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B");
                break;
            }
            else
            {
                itr++;
                continue;
            }
        }
    }
    //just for test

    void set_current_line_info(LineInfoValue info)
    {
        m_pCurrentLineInfoValue = info;
    }

    LineInfoValue get_current_line_info()
    {
        return m_pCurrentLineInfoValue;
    }


private:
    LineInfoValue m_pCurrentLineInfoValue;
    QMap<QString,QString> ConfigInfo;
    QMap<int,QString> m_pManualSN_A;
    QMap<int,QString> m_pManualSN_B;
    quint8 process_ip_string(const QString ipstr);
    QString search_http_config_file();
    void del_one_change_product_info(QString machinename);
    QList<QString> ListBoardInfoA;
    QList<QString> ListBoardInfoB;

    QList<QString> m_pMachineNamesOfChangeProduction;

    QFile m_pConfigFile;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;
    QString m_pConfigFileName;
    QString board_A_serial_number;
    QString board_B_serial_number;
    int boardSign;
    DataCommSerial* pComm;
    //notice PLC or AOI the current change production info
    QMap<QString,QMap<QString,QString>> mapOneButtonToVisionOrAOI;//QString:machineStationName QString:boardStyle QString:halfMaterialNum
    //notice PLC or AOI the current change production info

    //just for tv test,delete later
    bool test_tv_change_sign;
    //just for tv test,delete later

public:
    //just for tv test,delete later
    bool get_tv_change_sign()
    {
        return test_tv_change_sign;
    }
    void set_tv_change_sign(bool tmpsign)
    {
        test_tv_change_sign = tmpsign;
    }
    //just for tv test,delete later
    static GDataFactory* get_factory()
    {
        if(m_pGDataFactory == nullptr)
            m_pGDataFactory = new GDataFactory();
        return m_pGDataFactory;
    }

    static MainWindow* get_main_window()
    {
        if(m_pMainWindow == nullptr)
            m_pMainWindow = new MainWindow();
        return m_pMainWindow;
    }

    static WelcomeWidget* get_welcome_wgt()
    {
        if(m_pWelcomeWgt == nullptr)
            m_pWelcomeWgt = new WelcomeWidget();
        return m_pWelcomeWgt;
    }

    static DMDBAccessInterface* get_pgsql()
    {
        if(m_pDMDBAccess == nullptr)
            m_pDMDBAccess = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccess;
    }

    static CentralWidget* get_central_wgt()
    {
        if(m_pCentralWgt == nullptr)
            m_pCentralWgt = new CentralWidget();
        return m_pCentralWgt;
    }

    static LeftWidget* get_left_wgt()
    {
        if(m_pLeftWgt == nullptr)
            m_pLeftWgt = new LeftWidget();
        return m_pLeftWgt;
    }

    static RightHeadWidget* get_right_head_wgt()
    {
        if(m_pRightHeadWgt == nullptr)
            m_pRightHeadWgt = new RightHeadWidget();
        return m_pRightHeadWgt;
    }

    static RightMainWindow* get_right_main_window()
    {
        if(m_pRightMainWindow == nullptr)
            m_pRightMainWindow = new RightMainWindow();
        return m_pRightMainWindow;
    }

    static RightMainWidget* get_right_main_wgt()
    {
        if(m_pRightMainWgt == nullptr)
            m_pRightMainWgt = new RightMainWidget();
        return m_pRightMainWgt;
    }

    static RightQueryWidget* get_right_query_wgt()
    {
        if(m_pRightQueryWgt == nullptr)
            m_pRightQueryWgt = new RightQueryWidget();
        return m_pRightQueryWgt;
    }

    static TestWidget* get_test_wgt()
    {
        if(m_pTestWgt == nullptr)
            m_pTestWgt = new TestWidget();
        return m_pTestWgt;
    }

    static MesProcess* get_mes_process()
    {
        if(m_pMesProcess == nullptr)
            m_pMesProcess = new MesProcess();
        return m_pMesProcess;
    }

    static TestPlanWgt* get_test_plan_wgt()
    {
        if(m_pTestPlanWgt == nullptr)
            m_pTestPlanWgt = new TestPlanWgt();
        return m_pTestPlanWgt;
    }

    static BoardStyleWidget* get_board_style_wgtA()
    {
        if(m_pBoardStyleWgtA == nullptr)
            m_pBoardStyleWgtA = new BoardStyleWidget(nullptr,u8"板A");
        return m_pBoardStyleWgtA;
    }

    static BoardStyleWidget* get_board_style_wgtB()
    {
        if(m_pBoardStyleWgtB == nullptr)
            m_pBoardStyleWgtB = new BoardStyleWidget(nullptr,u8"板B");
        return m_pBoardStyleWgtB;
    }

    static BoardStyleWidgetPd* get_board_style_wgt_pd()
    {
        if(m_pBoardStyleWgtPd == nullptr)
            m_pBoardStyleWgtPd = new BoardStyleWidgetPd(nullptr,u8"请添加换产机台");
        return m_pBoardStyleWgtPd;
    }

    static BoardStyleWidgetTv* get_board_style_wgt_tv()
    {
        if(m_pBoardStyleWgtTv == nullptr)
            m_pBoardStyleWgtTv = new BoardStyleWidgetTv(nullptr,u8"请添加换产机台");
        return m_pBoardStyleWgtTv;
    }

    HttpMsgProcess* get_http_process_obj(QString msgType)
    {
        HttpMsgProcess* httpMsgProcess = HttpMsgProcess::get_interface_obj(msgType);
        return httpMsgProcess;
    }

    static QsLogWidget* get_logs_widget(){
        if(m_pQsLogWgt == nullptr)
            m_pQsLogWgt = new QsLogWidget();
        return m_pQsLogWgt;
    }

    static HistoryDisplayWgt* get_history_display_wgt()
    {
        if(m_pHistoryDisplayWgt == nullptr)
            m_pHistoryDisplayWgt = new HistoryDisplayWgt();
        return m_pHistoryDisplayWgt;
    }

    static ElecEquipTestPlanWgt* get_elec_equip_test_plan_wgt()
    {
        if(m_pElecEquipWgt == nullptr)
            m_pElecEquipWgt = new ElecEquipTestPlanWgt();
        return m_pElecEquipWgt;
    }

    static StationTabWgt* get_station_tab_wgt()
    {
        if(m_pStationTabWgt == nullptr)
            m_pStationTabWgt = new StationTabWgt();
        return m_pStationTabWgt;
    }

    static UserTabWgt* get_user_tab_wgt()
    {
        if(m_pUserTabWgt == nullptr)
            m_pUserTabWgt = new UserTabWgt();
        return m_pUserTabWgt;
    }

    static StationMonitorWgt* get_station_monitor_wgt()
    {
        if(m_pStationMonitorWgt == nullptr)
            m_pStationMonitorWgt = new StationMonitorWgt();
        return m_pStationMonitorWgt;
    }

    static StaticFileController* get_static_file_controller()
    {
        //the obj m_pStaticFileController is special here,so the writting
        //pattern is ABNORMAL
        if(m_pStaticFileController == nullptr)
            return nullptr;
        else
            return m_pStaticFileController;
    }

    static DBProcessObj_pd* get_DB_process_pd()
    {
        if(m_pDBProcessObj_PD == nullptr)
            m_pDBProcessObj_PD = new DBProcessObj_pd;
        return m_pDBProcessObj_PD;
    }

    static TvPlanWgt* get_tv_plan_wgt()
    {
        if(m_pTvPlanWgt == nullptr)
            m_pTvPlanWgt = new TvPlanWgt();
        return m_pTvPlanWgt;
    }
private:
    static GDataFactory* m_pGDataFactory;
    static MainWindow* m_pMainWindow;
    static WelcomeWidget* m_pWelcomeWgt;
    static DMDBAccessInterface* m_pDMDBAccess;
    static CentralWidget* m_pCentralWgt;
    static LeftWidget* m_pLeftWgt;
    static RightHeadWidget* m_pRightHeadWgt;
    static RightMainWindow* m_pRightMainWindow;
    static RightMainWidget* m_pRightMainWgt;
    static RightQueryWidget* m_pRightQueryWgt;
    static TestWidget* m_pTestWgt;
    static MesProcess* m_pMesProcess;
    static TestPlanWgt* m_pTestPlanWgt;
    static BoardStyleWidget* m_pBoardStyleWgtA;
    static BoardStyleWidget* m_pBoardStyleWgtB;
    static QsLogWidget* m_pQsLogWgt;
    static HistoryDisplayWgt* m_pHistoryDisplayWgt;
    static TvPlanWgt* m_pTvPlanWgt;

    static ElecEquipTestPlanWgt* m_pElecEquipWgt;
    static StationTabWgt* m_pStationTabWgt;
    static UserTabWgt* m_pUserTabWgt;
    static StationMonitorWgt* m_pStationMonitorWgt;
    static StaticFileController* m_pStaticFileController;
    static DBProcessObj_pd* m_pDBProcessObj_PD;
    static BoardStyleWidgetPd* m_pBoardStyleWgtPd;
    static BoardStyleWidgetTv* m_pBoardStyleWgtTv;
signals:
    void signal_change_production_already_complete();
    void signal_register_new_station_success();
public slots:
    void slot_rev_serial_number(const QByteArray&);
    void slot_change_production_failed(QString machinename,QString linename);
};

#endif // GDATAFACTORY_H
