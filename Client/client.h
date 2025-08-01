#ifndef CLIENT_H
#define CLIENT_H

#include "protocol.h"

#include <QTcpSocket>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:

    ~Client();
    void loadConfig();//在这个类中新增加载配置函数
    QString m_strIP;//定义IP成员变量
    quint16 m_usport;//定义端口号成员变量
    QTcpSocket m_tcpSocket;//定义连接服务器的成员变量 QTcpSocket 套接字
    static Client& getlnstance();//定义静态函数
    void sendMsg(PDU* pdu);//发送消息
    QString m_strLoginName;//存的当前登录用户名
    PDU* readMsg();
    void handleMsg(PDU* pdu);
    QString m_strRootPath;
    QByteArray buffer;

public slots:
    void showConnect();
    void recvMsg();

//private slots:
//    void on_pushButton_clicked();

private slots:
    void on_login_PB_clicked();

    void on_regist_PB_clicked();

private:
    Ui::Client *ui;
    Client(QWidget *parent = nullptr);//私有化构造函数
    Client(const Client& instance) = delete;//删除构造函数
    Client& operator=(const Client&) = delete;//删除拷贝赋值运算符
};
#endif // CLIENT_H
