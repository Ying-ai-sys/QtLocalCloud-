#include "client.h"
#include "index.h"
#include "protocol.h"
#include "reshandler.h"
#include "ui_client.h"
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include<QMessageBox>
Client::Client(QWidget *parent): QWidget(parent), ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();//调用加载配置的函数
//这个连接主要是客户端连接服务器的时候，为了验证是否连接成功，打印创建的槽函数和信号
    connect(&m_tcpSocket,&QTcpSocket::connected,this,&Client::showConnect);
  //构造函数中关联信号槽，客户端接收响应的
    connect(&m_tcpSocket,&QTcpSocket::readyRead,this,&Client::recvMsg);
//客户端连接服务器，调用的qtcpsever里面的connecttohost函数，参数传入ip和端口号
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usport);//调用连接服务器函数
}

Client::~Client()
{
    delete ui;
}

void Client::loadConfig()//加载配置的函数，将配置文件里的内容读取出来
{
    QFile file(":/client.config");//新建QFile对象 路径传参
    if(!file.open(QIODevice::ReadOnly)){//用只读形式打开
        qDebug()<<"Client 打开配置文件失败";
        return;
    }
    QByteArray baData =file.readAll();//将内容读取出来
    qDebug()<<baData;
    QString strData = QString(baData);//转换为字符串类型
    QStringList strlist = strData.split("\r\n");//截断
    m_strIP = strlist.at(0);//字符串第一个字符是ip地址
    m_usport = strlist.at(1).toUShort();//字符串第二个字符是端口号
    m_strRootPath = strlist.at(2);
    qDebug()<<"ip:"<<m_strIP<<"port:"<<m_usport<<"m_strRootPath:"<<m_strRootPath;//输出
    file.close();//关闭文件
}

Client &Client::getlnstance()
{
    static Client instance;//创建并返回一个静态局部对象
    return instance;
}

void Client::sendMsg(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    m_tcpSocket.write((char*)pdu,pdu->uiPDULen);//客户端和服务器通信，发送
    qDebug()<<"sendMsg pdu->uiPDULen"<<pdu->uiPDULen
            <<"pdu->uiMsgLen"<<pdu->uiMsgLen
            <<"pdu->uiMsgType"<<pdu->uiMsgType
            <<"pdu->caData"<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32
            <<"pdu->caMsg"<<pdu->caMsg;

    free(pdu);//结束之后申请的空间要释放
    pdu=NULL;
}

PDU *Client::readMsg()
{
    qDebug() <<"readMsg 接收消息长度"<<m_tcpSocket.bytesAvailable();
    //读取协议总长度
    uint uiPDULen=0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));
    //读取PDU中协议总长度以外的其他内容
    uint uiMsgLen=uiPDULen-sizeof(PDU);
    PDU* pdu=mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    qDebug()<<"readMsg pdu->uiPDULen"<<pdu->uiPDULen
            <<"pdu->uiMsgLen"<<pdu->uiMsgLen
            <<"pdu->uiMsgType"<<pdu->uiMsgType
            <<"pdu->caData"<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32
            <<"pdu->caMsg"<<pdu->caMsg;
    return pdu;
}

void Client::handleMsg(PDU *pdu)
{
    ResHandler rh(pdu);
    switch(pdu->uiMsgType){
    case ENUM_MSG_TYPE_REGIST_RESPOND:{
       rh.regist();
       break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:{
        rh.login();
        break;
    }
    case ENUM_MSG_TYPE_FIND_USER_RESPOND:{
        rh.findUser();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
        rh.addFriend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESEND:{
        rh.addFriendResend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND:{
        rh.addFriendAgree();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        rh.flushFriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{
        rh.chat();
        break;
    }
    case ENUM_MSG_TYPE_MKDIR_RESPOND:{
        rh.mkdir();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:{
        rh.flushFile();
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:{
        rh.moveFile();
        break;
    }
    case ENUM_MSG_TYPE_UOLOAD_FILE_INIT_RESPOND:{
        rh.uploadFileInit();
        break;
    }
    case ENUM_MSG_TYPE_UOLOAD_FILE_DATA_RESPOND:{
        Index::getInstance().getFile()->flushFile();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:{
        rh.shareFile();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
        rh.shareFileResend();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND:{
        rh.shareFileAgree();
        break;
    }
    default:
        break;
    }
    pdu=NULL;
}

//为了验证客户端和服务器是否连接成功，创建的槽函数，主要是连接成功后会发送一个信号，从而触发这个槽函数，打印连接成功
void Client::showConnect()
{
    qDebug()<<"客户端连接成功";
}
//客户端接收消息的函数
void Client::recvMsg()
{
    qDebug() <<"readMsg 接收消息长度"<<m_tcpSocket.bytesAvailable();
    QByteArray data = m_tcpSocket.readAll();
    buffer.append(data);
    while(buffer.size() >= int(sizeof(PDU))){
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < int(pdu->uiPDULen)){
            break;
        }
        handleMsg(pdu);
        buffer.remove(0,pdu->uiPDULen);
    }

}

//注册按钮的槽函数
void Client::on_login_PB_clicked()
{
//从输入框获取用户名和密码
    QString strName=ui->username_LE->text();
    QString strPwd=ui->pwd_LE->text();
//判断输入的账号密码长度是否合法
    if(strName.toStdString().size()>32||strName.isEmpty()
            ||strPwd.toStdString().size()>32||strPwd.isEmpty()){
        QMessageBox::information(this,"注册","用户名或密码长度非法");
        return;
    }
//构建PDU
    PDU* pdu=mkPDU();
//pdu类型赋枚举值为注册请求
    pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
//用户名和密码填到pdu的caData，用复制memcpy
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
//发送给服务器
    sendMsg(pdu);
}

//登录
void Client::on_regist_PB_clicked()
{
//从输入框获取用户名和密码
    QString strName=ui->username_LE->text();
    QString strPwd=ui->pwd_LE->text();
    PDU* pdu=mkPDU();
    pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    m_strLoginName = strName;
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    sendMsg(pdu);
}
