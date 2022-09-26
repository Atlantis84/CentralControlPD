#include "BoardStyleWidget_Tv.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTreeWidgetItem>
#include <QDragMoveEvent>
#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QAction>
#include "gdatafactory.h"
#include "QsLog.h"
#include "imessagebox.h"
BoardStyleWidgetTv::BoardStyleWidgetTv(QWidget *parent,QString boardName) : QWidget(parent)
{
    currentBoardName = boardName;
    setAcceptDrops(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);

    QGroupBox* grpBox = new QGroupBox(boardName);
    grpBox->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");

    hBox2 = new QHBoxLayout();
    grpContainer = new QGroupBox();
    grpContainer->setStyleSheet("background-color:rgba(0,0,0,50);");
    grpContainer->setLayout(hBox2);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vControls = new QVBoxLayout();
    vControls->addWidget(grpContainer);
    grpBox->setLayout(vControls);
    vAll->addWidget(grpBox);
    this->setLayout(vAll);

    pMenu = new QMenu();
    pMenu->setStyleSheet("QMenu::item{color:rgb(255,255,255);"
                         "font-family:Microsoft YaHei;"
                         "font-size:15px;"
                         "background-color:rgba(24,144,255,255);"
                         "}");
    pDeleteTask = new QAction(u8"É¾³ý¶ÔÏó",this);
    pMenu->addAction(pDeleteTask);
    connect(pDeleteTask,SIGNAL(triggered()),this,SLOT(slot_delete_label()));
}

void BoardStyleWidgetTv::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
//    painter.drawRect(this->rect());
}

void BoardStyleWidgetTv::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QByteArray data = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

RightLabel* BoardStyleWidgetTv::createDragLabel(const QString &text, QWidget *parent)
{
    RightLabel *label=nullptr;
    bool foundSign = false;
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == text)
            foundSign = true;
        else
            continue;
    }

    if(foundSign)
        return nullptr;
    else
    {
        label = new RightLabel(text, parent);
        label->setAutoFillBackground(true);
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Raised);
        connect(label,SIGNAL(signal_right_click(QString)),this,SLOT(slot_on_label_click(QString)));
    }
    return label;
}

void BoardStyleWidgetTv::slot_on_label_click(QString strText)
{
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == strText)
        {
            strToDelete = strText;
            pMenu->exec(cursor().pos());
        }
    }
}

void BoardStyleWidgetTv::slot_delete_label()
{
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == strToDelete)
        {
            hBox2->removeWidget(tmpWgt);
            tmpWgt->setParent(nullptr);
        }
        else
            continue;
    }
}

void BoardStyleWidgetTv::slot_comb_index_changed(int id)
{

}

static QString hotSpotMimeDataKey() { return QStringLiteral("application/x-qabstractitemmodeldatalist"); }

void BoardStyleWidgetTv::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        const QMimeData *mime = event->mimeData();
        QStringList pieces = mime->text().split(QRegularExpression(QStringLiteral("\\s+")),
                             QString::SkipEmptyParts);
//        QPoint position = event->pos();
        QPoint hotSpot;

        QByteArrayList hotSpotPos = mime->data(hotSpotMimeDataKey()).split(' ');
        if (hotSpotPos.size() == 2) {
            hotSpot.setX(hotSpotPos.first().toInt());
            hotSpot.setY(hotSpotPos.last().toInt());
        }

        for (const QString &piece : pieces) {
            if(hBox2->count()>3)//if controls > 3 do not add controls
                break;
            else
            {
                RightLabel *newLabel = createDragLabel(piece, grpContainer);
                if(newLabel)
                {
                    newLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                    newLabel->show();
                    newLabel->setAttribute(Qt::WA_DeleteOnClose);
                    hBox2->addWidget(newLabel);
                }
            }
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
    for (QWidget *widget : findChildren<QWidget *>()) {
        if (!widget->isVisible())
            widget->deleteLater();
    }
}

void BoardStyleWidgetTv::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->load_pre_controls();
}

void BoardStyleWidgetTv::load_pre_controls()
{
    QStringList pieces;
    for (int i=1;i<4;i++)
    {
        QString strKey = QString("MACHINE_STATION_%1").arg(i);
        if(GDataFactory::get_factory()->get_config_para(strKey) != "")
            pieces.append(GDataFactory::get_factory()->get_config_para(strKey));
    }

    for (const QString &piece : pieces)
    {

        RightLabel *newLabel = createDragLabel(piece, grpContainer);
        if(newLabel)
        {
            newLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            newLabel->show();
            newLabel->setAttribute(Qt::WA_DeleteOnClose);
            hBox2->addWidget(newLabel);
        }
    }
}

void BoardStyleWidgetTv::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasText() || event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void BoardStyleWidgetTv::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void BoardStyleWidgetTv::set_serial_number(const QString serialNum)
{
    this->lineEditSN->setText(serialNum);
}

void BoardStyleWidgetTv::save_plan_info()
{
    for(int i=1;i<5;i++)
        {
            QString tmpStr = QString("MACHINE_STATION_%1").arg(i);
            GDataFactory::get_factory()->set_config_para_1(tmpStr,"");
        }

        for (int i=0;i<hBox2->count();i++)
        {
            QLayoutItem* it = hBox2->layout()->itemAt(i);
            RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
            QString keyStr = QString("MACHINE_STATION_%1").arg(i+1);
            GDataFactory::get_factory()->set_config_para_1(keyStr,tmpWgt->text());
            //for pingdu change produciton
            GDataFactory::get_factory()->add_change_production_machine_name(tmpWgt->text());
            //for pingdu change produciton
        }

        GDataFactory::get_factory()->save_config_file();
}
