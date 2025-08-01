#include "mytcpsocket.h"
#include"protocol.h"
#include"operatedb.h"
#include "reqhandler.h"
MyTcpSocket::MyTcpSocket()
{

}

PDU *MyTcpSocket::readMsg()//读取消息封装函数
{
    qDebug() <<"recvMsg 接收消息长度"<<this->bytesAvailable();
    //读取协议总长度
    uint uiPDULen=0;
    this->read((char*)&uiPDULen,sizeof(uint));
    //读取PDU中协议总长度以外的其他内容
    uint uiMsgLen=uiPDULen-sizeof(PDU);
    PDU* pdu=mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));

    qDebug()<<"readMsg pdu->uiPDULen"<<pdu->uiPDULen
            <<"pdu->uiMsgLen"<<pdu->uiMsgLen
            <<"pdu->uiMsgType"<<pdu->uiMsgType
            <<"pdu->caData"<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32
            <<"pdu->caMsg"<<pdu->caMsg;
    return pdu;
}

PDU *MyTcpSocket::handleMsg(PDU *pdu)//处理消息封装函数
{
    if(pdu == NULL){
        return NULL;
    }
    rh.pdu = pdu;
    PDU* respdu = NULL;
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_REQUEST:{//注册
        respdu = rh.regist();
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:{//登录
        respdu = rh.login(m_strLoginName);
        break;
    }
    case ENUM_MSG_TYPE_FIND_USER_REQUEST:{//查找
        respdu = rh.findUser();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        respdu=rh.addFriend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:{
        respdu=rh.addFriendAgree();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:{
        respdu=rh.flushFriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{
        respdu=rh.chat();
        break;
    }
    case ENUM_MSG_TYPE_MKDIR_REQUEST:{
        respdu=rh.mkdir();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:{
        respdu=rh.flushFile();
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:{
        respdu=rh.moveFile();
        break;
    }
    case ENUM_MSG_TYPE_UOLOAD_FILE_INIT_REQUEST:{
        respdu=rh.uploadFile();
        break;
    }
    case ENUM_MSG_TYPE_UOLOAD_FILE_DATA_REQUEST:{
        respdu=rh.uploadFileData();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
        respdu=rh.ShareFile();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:{
        respdu=rh.ShareFileAgree();
        break;
    }
    default:
        break;
    }
    return respdu;
}

void MyTcpSocket::sendMsg(PDU *pdu)//发送消息封装函数
{
    if(pdu == NULL){
        return;
    }
    this->write((char*)pdu,pdu->uiPDULen);
    qDebug()<<"sendMsg pdu->uiPDULen"<<pdu->uiPDULen
            <<"pdu->uiMsgLen"<<pdu->uiMsgLen
            <<"pdu->uiMsgType"<<pdu->uiMsgType
            <<"pdu->caData"<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32
            <<"pdu->caMsg"<<pdu->caMsg;
    free(pdu);
    pdu=NULL;
}

void MyTcpSocket::recvMsg()//在这里接收,接收消息函数
{
    qDebug() <<"recvMsg 接收消息长度"<<this->bytesAvailable();
    QByteArray data = this->readAll();//读取socket全部数据
    buffer.append(data);//读取到的socket数据添加到buffer中
    while(buffer.size() >= int(sizeof (PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(pdu->uiPDULen)){
            break;
        }
        PDU* respdu=handleMsg(pdu);
        qDebug()<<"recvMsg pdu->uiPDULen"<<pdu->uiPDULen
                <<"pdu->uiMsgLen"<<pdu->uiMsgLen
                <<"pdu->uiMsgType"<<pdu->uiMsgType
                <<"pdu->caData"<<pdu->caData
                <<"pdu->caData+32"<<pdu->caData+32
                <<"pdu->caMsg"<<pdu->caMsg;
        sendMsg(respdu);
        buffer.remove(0,pdu->uiPDULen);
    }
}
//下线会自动发送disconnected这个信号，从而触发这个槽函数，在构造函数中有连接这个信号和槽函数
void MyTcpSocket::clientOffline()
{
    OperateDB::getInstance().handleOffline(m_strLoginName.toStdString().c_str());
    emit offline(this);//发送信号
}


