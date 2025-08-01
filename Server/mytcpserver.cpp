#include "clienttask.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"
MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}
//槽函数：将要下线的socket 在列表中移除
void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    m_tcpSocktlist.removeOne(mysocket);//从列表中移除
    mysocket->deleteLater();//延迟释放
    mysocket = NULL;
    //测试
    for(int i=0;i<m_tcpSocktlist.size();i++){
        qDebug()<<m_tcpSocktlist.at(i)->m_strLoginName;//打印一下列表里还有谁的名字
    }
}

void MyTcpServer::resend(char *tarName, PDU *pdu)
{
    if(tarName == NULL || pdu == NULL){
        return;
    }
    foreach(MyTcpSocket* pSocket,m_tcpSocktlist){
        if(pSocket->m_strLoginName == tarName){
            pSocket->write((char*)pdu,pdu->uiPDULen);
            qDebug()<<"resend pdu->uiPDULen"<<pdu->uiPDULen
                    <<"pdu->uiMsgLen"<<pdu->uiMsgLen
                    <<"pdu->uiMsgType"<<pdu->uiMsgType
                    <<"pdu->caData"<<pdu->caData
                    <<"pdu->caData+32"<<pdu->caData+32
                    <<"pdu->caMsg"<<pdu->caMsg;
            break;
        }
    }
}

MyTcpServer::MyTcpServer()
{
    threadPool.setMaxThreadCount(8);//在构造函数中设置线程最大数量
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接成功";
    MyTcpSocket* pSocket = new MyTcpSocket;
    pSocket->setSocketDescriptor(handle);
    m_tcpSocktlist.append(pSocket);
    //连接MyTcpSocket发出的下线信号，和槽函数删除socket
    connect(pSocket,&MyTcpSocket::offline,this,&MyTcpServer::deleteSocket);
    Clienttask* task = new Clienttask(pSocket);
    threadPool.start(task);
}
