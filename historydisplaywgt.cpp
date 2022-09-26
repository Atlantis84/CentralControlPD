#include "historydisplaywgt.h"
#include "TitleBar.h"
#include <QPainter>
#include "gdatafactory.h"
#include "QsLog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QTabWidget>
HistoryDisplayWgt::HistoryDisplayWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(600,600);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"数据管理");
    pTitleBar->setFixedWidth(600);
    installEventFilter(pTitleBar);

    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->addTab(GDataFactory::get_station_tab_wgt(),u8"用户数据管理");
//    tabWidget->addTab(GDataFactory::get_user_tab_wgt(),u8"用户数据管理");

    QVBoxLayout* vAll = new QVBoxLayout();
//    vAll->setMargin(0);
//    vAll->setSpacing(0);
    vAll->addSpacing(40);
    vAll->addWidget(tabWidget);
    this->setLayout(vAll);
}

void HistoryDisplayWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}
