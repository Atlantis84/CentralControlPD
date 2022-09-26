#include "stationtabwgt_pd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSqlQuery>
#include "gdatafactory.h"
#include "QsLog.h"
#include "imessagebox.h"
//    QDateEdit* editDate= new QDateEdit();
//    editDate->setDateTime(QDateTime::currentDateTime());
//    editDate->setDisplayFormat("yyyy/MM/dd");
//    editDate->setStyleSheet("QDateEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
//                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
//                                   font-size: 20px;font-weight:normal;\
//                                   border-radius:0px;min-width:130px;\
//                                   selection-background-color:rgba(0,0,0,100);\
//                                   selection-color:#5D5C6C;}\
//                                   ");
StationTabWgt::StationTabWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    QVBoxLayout* vBox2 = new QVBoxLayout;
    QLabel* labelUserName = new QLabel(u8"用户名:");
    lineEditUserName = new QLineEdit;
    lineEditUserName->setAlignment(Qt::AlignCenter);
    lineEditUserName->setStyleSheet("QLineEdit{color:rgba(0,0,0,100);border:1px solid rgba(0,0,0,100);\
                                       border-radius:5px;min-width:100px;\
                                       background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                       font-size: 20px;font-weight:normal;\
                                       }");
    QPushButton* btnAddUser = new QPushButton(u8"  添 加 用 户  ");
    btnAddUser->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-height:30px;min-width:100px;}\
                                QPushButton:!enabled{border:1px solid rgba(0,0,0,30);}\
                                QPushButton:hover{color:rgb(255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnAddUser,SIGNAL(clicked()),this,SLOT(slot_add_user()));

    QLabel* labelPassword = new QLabel(u8"   密码:");
    lineEditPassword = new QLineEdit;
    lineEditPassword->setAlignment(Qt::AlignCenter);
    lineEditPassword->setStyleSheet("QLineEdit{color:rgba(0,0,0,100);border:1px solid rgba(0,0,0,100);\
                                       border-radius:5px;min-width:100px;\
                                       background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                       font-size: 20px;font-weight:normal;\
                                       }");
    QPushButton* btnDeleteUser = new QPushButton(u8"  删 除 用 户  ");
    btnDeleteUser->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-height:30px;min-width:100px;}\
                                    QPushButton:!enabled{border:1px solid rgba(0,0,0,30);}\
                                    QPushButton:hover{color:rgb(255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnDeleteUser,SIGNAL(clicked()),this,SLOT(slot_delete_user()));

    QLabel* labelUserPermsn = new QLabel(u8"  权限:");
    cmbUserPermsn = new QComboBox;
    cmbUserPermsn->insertItem(0,u8"管理员");
    cmbUserPermsn->insertItem(1,u8"普通用户");
    cmbUserPermsn->setCurrentIndex(-1);
    cmbUserPermsn->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-width:150px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    connect(cmbUserPermsn,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));
    QPushButton* btnEditUser = new QPushButton(u8"  修 改 用 户  ");
    connect(btnEditUser,SIGNAL(clicked()),this,SLOT(slot_edit_user()));
    btnEditUser->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-height:30px;min-width:100px;}\
                                        QPushButton:!enabled{border:1px solid rgba(0,0,0,30);}\
                                        QPushButton:hover{color:rgb(255,255,255);background-color:rgba(24,144,255,255);}");

    QHBoxLayout* hBox1 = new QHBoxLayout;
    hBox1->addStretch(1);
    hBox1->addWidget(labelUserName);
    hBox1->addWidget(lineEditUserName);
    hBox1->addStretch(1);
    hBox1->addWidget(btnAddUser);
    hBox1->addStretch(1);
    QHBoxLayout* hBox2 = new QHBoxLayout;
    hBox2->addStretch(1);
    hBox2->addWidget(labelPassword);
    hBox2->addWidget(lineEditPassword);
    hBox2->addStretch(1);
    hBox2->addWidget(btnEditUser);
    hBox2->addStretch(1);
    QHBoxLayout* hBox3 = new QHBoxLayout;
    hBox3->addStretch(1);
    hBox3->addWidget(labelUserPermsn);
    hBox3->addSpacing(10);
    hBox3->addWidget(cmbUserPermsn);
    hBox3->addStretch(1);
    hBox3->addWidget(btnDeleteUser);
    hBox3->addStretch(1);

    vBox2->addLayout(hBox1);
    vBox2->addLayout(hBox2);
    vBox2->addLayout(hBox3);
//    vAll->addWidget(grpBox);
    vAll->addWidget(createTableWidget());
    vAll->addLayout(vBox2);
//    vAll->setStretch(0,1);
//    vAll->setStretch(1,4);
    this->setLayout(vAll);
}

QWidget *StationTabWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(20,3);

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"用户名"<<u8"密码"<<u8"权限";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{font-family:Microsoft YaHei;border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QTableWidget::SelectRows);

    connect(tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slot_item_selected(QTableWidgetItem*)));
    return tableWidget;
}

void StationTabWgt::set_table_data()
{
    this->tableWidget->clearContents();
    QString strSql = QString("select * from public.%1").arg(constUserInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        int i=0;
        while(queryResult.next())
        {
            QString usrPermission;
            if(queryResult.value(3).toString() == "administrator")
                usrPermission = u8"管理员";
            else
                usrPermission = u8"普通用户";
            QTableWidgetItem *item1 = new QTableWidgetItem(queryResult.value(1).toString());
            QTableWidgetItem *item2 = new QTableWidgetItem(GDataFactory::get_factory()->DecodeStr(queryResult.value(2).toString()));
            QTableWidgetItem *item3 = new QTableWidgetItem(usrPermission);
            item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
            item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
            item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
            tableWidget->setItem(i,0,item1);
            tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->setItem(i,1,item2);
            tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->setItem(i,2,item3);
            tableWidget->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            i++;
        }
    }
}

void StationTabWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    set_table_data();
}

void StationTabWgt::slot_edit_user()
{
    QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();

    if(items.size() == 0)
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"修改用户前,请在表中选择要修改的用户!");
        return;
    }

    if((this->lineEditPassword->text().trimmed() == "") ||
            (this->lineEditUserName->text().trimmed() == "") ||
            (this->cmbUserPermsn->currentText().trimmed() == ""))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"修改用户,数据不健全!");
        return;
    }

    QString strName,strPassword,strID;
    for(int i=0;i<items.count();i++)
    {
        QTableWidgetItem* item = items.at(i);
        if(i==0)
            strName = item->text();
        else if(i==1)
            strPassword = GDataFactory::get_factory()->EncodeStr(item->text());
        else
        {
            if(item->text() == u8"管理员")
                strID = "administrator";
            else
                strID = "averageuser";
        }
    }
    QString tmpID;
    if(this->cmbUserPermsn->currentText() == u8"管理员")
        tmpID = "administrator";
    else
        tmpID = "averageuser";

    QString strSql = QString("update %1 set usrname = '%2',password = '%3',usrpermission = '%4'"
                             "where usrname = '%5' and password = '%6' and usrpermission ='%7'").
            arg(constUserInfoTable).
            arg(this->lineEditUserName->text()).
            arg(GDataFactory::get_factory()->EncodeStr(this->lineEditPassword->text())).
            arg(tmpID).
            arg(strName).
            arg(strPassword).
            arg(strID);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
    {
        QLOG_INFO()<<"update user table SUCCESS!";
        set_table_data();
    }
    else {
        QLOG_WARN()<<"update user table FAILED!";
    }
}

void StationTabWgt::slot_delete_user()
{
    QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();

    if(items.size() == 0)
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"删除用户前,请在表中选择要删除的用户!");
        return;
    }

    QString strName,strPassword,strID;
    for(int i=0;i<items.count();i++)
    {
        QTableWidgetItem* item = items.at(i);
        if(i==0)
            strName = item->text();
        else if(i==1)
            strPassword = GDataFactory::get_factory()->EncodeStr(item->text());
        else
        {
            if(item->text() == u8"管理员")
                strID = "administrator";
            else
                strID = "averageuser";
        }
    }

    QString strSql = QString("delete from %1 where usrname = '%2' and password = '%3' and usrpermission = '%4'").
            arg(constUserInfoTable).
            arg(strName).
            arg(strPassword).
            arg(strID);
    QString strErrorMsg;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
    {
        QLOG_INFO()<<"delete from user table SUCCESS!";
        set_table_data();
    }
    else{
        QLOG_WARN()<<"delete from user table FAILED!";
    }
}

void StationTabWgt::slot_add_user()
{
    if((this->lineEditPassword->text().trimmed() == "") ||
            (this->lineEditUserName->text().trimmed() == "") ||
            (this->cmbUserPermsn->currentText().trimmed() == ""))
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"添加用户,数据不健全!");
        return;
    }

    QString pwdStr = GDataFactory::get_factory()->EncodeStr(this->lineEditPassword->text());
    QString strSql = QString("select * from public.%1 where usrname = '%2' and password = '%3'").
            arg(constUserInfoTable).arg(this->lineEditUserName->text()).arg(pwdStr);
    QString usrID;
    if(this->cmbUserPermsn->currentText() == u8"管理员")
        usrID = "administrator";
    else
        usrID = "averageuser";

    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        if(queryResult.size() == 0)
        {
            int maxID = GDataFactory::get_factory()->get_biggest_id(constUserInfoTable);
            strSql = QString("insert into %1 values(%2,'%3','%4','%5')").
                    arg(constUserInfoTable).
                    arg(maxID).
                    arg(this->lineEditUserName->text()).
                    arg(pwdStr).
                    arg(usrID);
            QString strErrorMsg;
            if(GDataFactory::get_pgsql()->ExecSql(strSql,strErrorMsg))
            {
                QLOG_INFO()<<"insert user info into user table SUCCESS";
                set_table_data();
            }
            else
                QLOG_WARN()<<"insert user info into user table FAILED";
        }
        else
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"当前用户名与密码已存在!");
        }
    }
}

void StationTabWgt::slot_item_selected(QTableWidgetItem *it)
{
    Q_UNUSED(it);
    QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();

    if(items.count() == 3)
    {
        for(int i=0;i<items.count();i++)
        {
            QTableWidgetItem* item = items.at(i);
            if(i == 0)
                this->lineEditUserName->setText(item->text());
            else if(i == 1)
                this->lineEditPassword->setText(item->text());
            else
            {
                if(item->text() == u8"管理员")
                    this->cmbUserPermsn->setCurrentIndex(0);
                else
                    this->cmbUserPermsn->setCurrentIndex(1);
            }
        }
    }
}
