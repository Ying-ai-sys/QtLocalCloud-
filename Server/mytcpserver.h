#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "mytcpsocket.h"

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    QList<MyTcpSocket*>m_tcpSocktlist;//存储新建的socket的列表
    void deleteSocket(MyTcpSocket* mysocket);
    void resend(char *tarName,PDU *pdu);
    QThreadPool threadPool;//创建线程池
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& instance) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
//重写连接成功需要调用的函数
    void incomingConnection(qintptr handle) override;
};

#endif // MYTCPSERVER_H
