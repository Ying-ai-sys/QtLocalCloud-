#include "client.h"
#include "friend.h"
#include "protocol.h"
#include "ui_friend.h"

#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    flushFriend();
    m_pChat = new Chat;
}

Friend::~Friend()
{
    delete ui;
    delete m_pChat;
}

void Friend::flushFriend()
{
    QString strName = Client::getlnstance().m_strLoginName;
    PDU* pdu=mkPDU();
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    Client::getlnstance().sendMsg(pdu);
}

void Friend::updateFriend_LW(QStringList friendList)
{
    ui->frien_LW->clear();
    ui->frien_LW->addItems(friendList);//添加到好友列表框里
}

QListWidget *Friend::getFriend_LW()
{
    return ui->frien_LW;
}

void Friend::on_findUser_PB_clicked()
{
    QString strName = QInputDialog::getText(this,"搜索","用户名");
    if(strName.toStdString().size()>32||strName.isEmpty()){
        QMessageBox::information(this,"查找","用户名长度非法");
        return;
    }
    PDU* pdu =mkPDU();
    pdu->uiMsgType=ENUM_MSG_TYPE_FIND_USER_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    Client::getlnstance().sendMsg(pdu);
}

void Friend::on_flush_PB_clicked()//刷新好友的槽函数
{
    flushFriend();//刷新好友封装函数
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem* pItem = ui->frien_LW->currentItem();//friend 列表框获取当前项
    if(!pItem){
        return ;
    }
    if(m_pChat->isHidden()){//如果聊天框隐藏就显示出来
        m_pChat->show();
    }
    m_pChat->m_strChatName = pItem->text();//把我点击的聊天人名存起来
    m_pChat->setWindowTitle(pItem->text());//设置窗口标题
}
