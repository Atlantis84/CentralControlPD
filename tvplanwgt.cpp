#include "tvplanwgt.h"
#include "TitleBar.h"
#include <QPainter>
#include "gdatafactory.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "imessagebox.h"
#include <QStyleFactory>
TvPlanWgt::TvPlanWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,400);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"TV换产计划");
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
    lineProductSN = new QLineEdit();
    lineProductSN->setReadOnly(true);
    lineProductSN->setStyleSheet("background-color:rgb(240,242,245);font-family:Microsoft YaHei;font-size:20px;");
    hBox1->addStretch(1);
    hBox1->addWidget(labelSN);
    hBox1->addWidget(lineProductSN);
    hBox1->addStretch(1);
    QGroupBox* grpBoxProduct = new QGroupBox(u8"请选择产品");
    grpBoxProduct->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");
    grpBoxProduct->setLayout(hBox1);
    vBox2->addWidget(grpBoxProduct);
    vBox2->addWidget(GDataFactory::get_board_style_wgt_tv());
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

void TvPlanWgt::set_serial_number(QString serialnum)
{
    this->lineProductSN->setText(serialnum);
    serialnum = "ZTP2053E5AE";
    GDataFactory::get_mes_process()->exec_http_get(serialnum);
}

void TvPlanWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void TvPlanWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_change_btn_icon();
    GDataFactory::get_factory()->stop_serial_port();
}

void TvPlanWgt::slot_save_test_plan()
{
    QMutexLocker lock(&m_mutex);
    IMessageBox* msgBox = new IMessageBox(3);
    if(m_pListChangeInfo.size() == 0)
        msgBox->warning(u8"未从Mes获取信息!");
    else {
        m_pListChangeInfo.push_back(cmbLineName->currentText());
        m_pListChangeInfo.push_back(cmbStationName->currentText());
        this->close();
    }
}

void TvPlanWgt::showEvent(QShowEvent *event)
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
