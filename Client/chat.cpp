#include "chat.h"
#include "client.h"
#include "protocol.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::updateShow_TE(QString strMsg)
{
    ui->show_TE->append(strMsg);
}

void Chat::on_send_PB_clicked()
{
    QString strMsg = ui->input_LE->text();//获取我发送的内容
    if(strMsg.isEmpty()){
        return ;
    }
    ui->input_LE->clear();
    PDU* pdu = mkPDU(strMsg.toStdString().size()+1);
    pdu->uiMsgType =ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData,Client::getlnstance().m_strLoginName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,m_strChatName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.toStdString().size());
    Client::getlnstance().sendMsg(pdu);
}
