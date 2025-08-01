#include "client.h"
#include "index.h"
#include "sharefile.h"
#include "ui_sharefile.h"

Sharefile::Sharefile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sharefile)
{
    ui->setupUi(this);
}

Sharefile::~Sharefile()
{
    delete ui;
}

void Sharefile::updataFriend_LW()
{
    ui->listWidget->clear();
    QListWidget* friend_LW =Index::getInstance().getFriend()->getFriend_LW();
    QStringList friendList;
    for(int i=0;i<friend_LW->count();i++){
        friendList.append(friend_LW->item(i)->text());
    }
    ui->listWidget->addItems(friendList);
}

void Sharefile::on_allSelected_PB_clicked()
{
    for(int i=0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(true);
    }
}

void Sharefile::on_cancelSelected_PB_clicked()
{
    for(int i=0;i<ui->listWidget->count();i++){
        ui->listWidget->item(i)->setSelected(false);
    }
}

void Sharefile::on_ok_PB_clicked()
{
    QString strCurName = Client::getlnstance().m_strLoginName; //想要分享的用户
    QString strFilePath = Index::getInstance().getFile()->m_strShareFileName;//想要分享的文件路径
    QList<QListWidgetItem*> pItems = ui->listWidget->selectedItems();
    int iFriendNum = pItems.size();
    PDU* pdu = mkPDU(iFriendNum*32+strFilePath.toStdString().size()+1);
    pdu->uiMsgType =ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(pdu->caData,strCurName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&iFriendNum,sizeof (int));
    for(int i=0;i<iFriendNum;i++){
        memcpy(pdu->caMsg+i*32,pItems[i]->text().toStdString().c_str(),32);
    }
    memcpy(pdu->caMsg+iFriendNum*32,strFilePath.toStdString().c_str(),strFilePath.toStdString().size());
    Client::getlnstance().sendMsg(pdu);
}
