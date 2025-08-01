#include "mytcpserver.h"
#include "server.h"
#include "ui_server.h"

#include <QFile>
#include <QDebug>
#include <qhostaddress.h>

Server::Server(QWidget *parent): QWidget(parent), ui(new Ui::Server)
{
    loadConfig();
    //客户端与服务器建立连接，服务器需要监听调用listen函数，监听之后如果连接成功会得到一个socket
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usport);
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

//加载配置文件
void Server::loadConfig()
{
    QFile file(":/server.config");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Server 打开配置文件失败";
        return ;
    }
    QByteArray baData = file.readAll();
    QString strData=QString(baData);
    QStringList strlist = strData.split("\r\n");
    m_strIP = strlist.at(0);
    m_usport = strlist.at(1).toUShort();
    m_strRootPath = strlist.at(2);
    qDebug()<<"loadConfig m_strIP"<<m_strIP<<"m_usport"<<m_usport<<"m_strRootPath"<<m_strRootPath;
    file.close();
}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

