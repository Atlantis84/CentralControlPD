#include "centralwidget.h"
#include <QPainter>
#include "gdatafactory.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hAll = new QHBoxLayout();
    QVBoxLayout* vLeft = new QVBoxLayout();
    QVBoxLayout* vRight = new QVBoxLayout();

    vLeft->addWidget(GDataFactory::get_left_wgt());

    vRight->addWidget(GDataFactory::get_right_head_wgt());
    vRight->addWidget(GDataFactory::get_right_query_wgt());
    vRight->addWidget(GDataFactory::get_test_wgt());
//    vRight->addWidget(GDataFactory::get_right_main_wgt());
    vRight->setStretch(0,2);
    vRight->setStretch(1,3);
    vRight->setStretch(2,25);

    hAll->addLayout(vLeft);
    hAll->addLayout(vRight);
    hAll->setStretch(0,4);
    hAll->setStretch(1,15);
    this->setLayout(hAll);
}

void CentralWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(240,242,245)));
}
