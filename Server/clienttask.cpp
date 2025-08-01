#include "clienttask.h"

#include <QThread>

Clienttask::Clienttask(QObject *parent) : QObject(parent)
{

}

Clienttask::Clienttask(MyTcpSocket *socket)
{
    mySocket = socket;
}

void Clienttask::run()
{
    connect(mySocket,&QTcpSocket::readyRead,mySocket,&MyTcpSocket::recvMsg);
    connect(mySocket,&QTcpSocket::disconnected,mySocket,&MyTcpSocket::clientOffline);
    mySocket->moveToThread(QThread::currentThread());//将socket移入当前线程
}
