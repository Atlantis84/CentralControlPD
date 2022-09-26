#include "elecequiptestplanwgt_pd.h"
#include "TitleBar.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include "imessagebox.h"
#include <QTimerEvent>
#include "gdatafactory.h"
#include <QStyleFactory>
ElecEquipTestPlanWgt::ElecEquipTestPlanWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,400);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"平度电装换产计划");
    pTitleBar->setFixedWidth(800);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hAll = new QHBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();
    QVBoxLayout* vBox2 = new QVBoxLayout();
    QVBoxLayout* vBox3 = new QVBoxLayout();

//    QLabel* labelScanCode = new QLabel(u8"请扫码");
//    labelScanCode->setStyleSheet("font-size:30pt");
//    labelScanCode->setAlignment(Qt::AlignCenter);
    QLabel* labelPicture = new QLabel();
    labelPicture->setPixmap(QPixmap(":/icon/code.jpg"));
    labelPicture->setAlignment(Qt::AlignCenter);
    vBox1->addStretch(1);
//    vBox1->addWidget(labelScanCode);
    vBox1->addWidget(labelPicture);
    vBox1->addStretch(1);

    QPushButton* btnSave = new QPushButton(u8"   保  存   ");
    connect(btnSave,SIGNAL(clicked()),this,SLOT(slot_save_test_plan()));
    btnSave->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelSN = new QLabel(u8"产品型号:");
    labelSN->setStyleSheet("border:none;");
    combProductStyle = new QComboBox();
    combProductStyle->insertItem(0,u8"8341");
    combProductStyle->insertItem(1,u8"7725");
    combProductStyle->insertItem(2,u8"8921");
    combProductStyle->insertItem(3,u8"1123");
    combProductStyle->setCurrentIndex(2);
    combProductStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                    "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-width:200px;}"
                                    "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
//    connect(combProductStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));
    hBox1->addStretch(1);
    hBox1->addWidget(labelSN);
    hBox1->addWidget(combProductStyle);
    hBox1->addStretch(1);
    QGroupBox* grpBoxProduct = new QGroupBox(u8"请选择产品");
    grpBoxProduct->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    grpBoxProduct->setLayout(hBox1);
    vBox2->addWidget(grpBoxProduct);
    vBox2->addWidget(GDataFactory::get_board_style_wgt_pd());
    vBox2->addWidget(btnSave);
    vBox2->setStretch(0,2);
    vBox2->setStretch(1,3);
    vBox2->setStretch(2,1);

    dragTreeWgt = new DragTreeWidget();
    dragTreeWgt->setFont(QFont(u8"Microsoft YaHei",12,QFont::Normal,false));
    vBox3->addWidget(dragTreeWgt);

    hAll->addLayout(vBox1);
    hAll->addLayout(vBox2);
    hAll->addLayout(vBox3);
    hAll->setStretch(0,3);
    hAll->setStretch(1,6);
    hAll->setStretch(2,3);

    vAll->addSpacing(40);
    vAll->addLayout(hAll);
    this->setLayout(vAll);
}

void ElecEquipTestPlanWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void ElecEquipTestPlanWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    killTimer(m_timer_ID);
    emit signal_change_btn_icon();
}

void ElecEquipTestPlanWgt::slot_save_test_plan()
{
    if(check_product_exist())
        ;
    else
        return;

    m_time_out = 0;
    m_timer_ID = startTimer(1000);
    if(GDataFactory::get_factory()->truncate_change_info_table())
        emit signal_save_plan();
    else {
        QLOG_WARN()<<"truncate change info table failed!";
        return;
    }
    GDataFactory::get_factory()->stop_serial_port();

    this->hide();
}

bool ElecEquipTestPlanWgt::check_product_exist()
{
    QList<QString> tmpLst = GDataFactory::get_DB_process_pd()->get_pd_change_file_name(this->combProductStyle->currentText());
    if(tmpLst.count() == 2)
    {
        for(int i=0;i<tmpLst.count();i++)
        {
            if(tmpLst[i].contains("GD"))
                GDataFactory::get_factory()->set_config_para_1("PROJECT_D_FILE",tmpLst[i]);
            else
                GDataFactory::get_factory()->set_config_para_1("CONFIG_FILE",tmpLst[i]);
        }
        return true;
    }
    else
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"当前选择产品配置文件或工单文件不存在!");
        GDataFactory::get_factory()->set_config_para_1("CONFIG_FILE","");
        GDataFactory::get_factory()->set_config_para_1("PROJECT_D_FILE","");
        return false;
    }
}

void ElecEquipTestPlanWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timer_ID)
    {
        m_time_out++;
        if(m_time_out == 500000)
        {
            killTimer(m_timer_ID);
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"换产超时!");
            emit signal_change_btn_icon();
        }
    }
}

void ElecEquipTestPlanWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    dragTreeWgt->clear();
    QList<QTreeWidgetItem*> items;
    QMap<QString,QList<QString>> mapAll = GDataFactory::get_factory()->get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    while(itr!= mapAll.end())
    {
        QTreeWidgetItem* tmpItem = new QTreeWidgetItem(dragTreeWgt,QStringList(itr.key()));
        QList<QString> lstChild = itr.value();
        for (int i=0;i<lstChild.size();i++) {
            QTreeWidgetItem* tmpChildItem = new QTreeWidgetItem(tmpItem,QStringList(lstChild[i]));
            tmpItem->addChild(tmpChildItem);
        }
        dragTreeWgt->addTopLevelItem(tmpItem);
        itr++;
    }

    dragTreeWgt->setHeaderHidden(true);
    dragTreeWgt->setStyle(QStyleFactory::create("windows"));
    dragTreeWgt->setStyleSheet("background-color:rgb(240,242,245);");
    dragTreeWgt->expandAll();
}

void ElecEquipTestPlanWgt::slot_kill_timeout_timer()
{
    killTimer(m_timer_ID);
}
