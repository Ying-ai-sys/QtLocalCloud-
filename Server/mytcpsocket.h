#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include "protocol.h"
#include "reqhandler.h"

#include <QObject>
#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ReqHandler rh;
    QString m_strLoginName;
    PDU* readMsg();
    PDU* handleMsg(PDU* pdu);
    void sendMsg(PDU* pdu);
    QByteArray buffer;

public slots://定义槽函数
    void recvMsg();
    void clientOffline();
signals:
    void offline(MyTcpSocket* mysocket);//定义一个下线的信号,参数传入的是当前要下线的socket
};

#endif // MYTCPSOCKET_H
