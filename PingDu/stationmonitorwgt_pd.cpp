#include "stationmonitorwgt_pd.h"
#include <QVBoxLayout>
#include "TitleBar.h"
#include <QPainter>
StationMonitorWgt::StationMonitorWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1600,1000);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"Õ¾µã¼àÊÓ");
    pTitleBar->setFixedWidth(1600);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout;
    QWebEngineView* view = new QWebEngineView();
    view->setUrl(QStringLiteral("https://dt.hisense.com/#/decisionScreen/index?bigScreenId=0eeefcbc-5098-11eb-bdb5-005056a99ec5"));
    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(view);
    this->setLayout(vAll);
}

void StationMonitorWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}
