#ifndef CLIENTTASK_H
#define CLIENTTASK_H

#include "mytcpsocket.h"

#include <QObject>
#include <QRunnable>

class Clienttask : public QObject , public QRunnable
{
    Q_OBJECT
public:
    explicit Clienttask(QObject *parent = nullptr);
    MyTcpSocket* mySocket;
    Clienttask(MyTcpSocket* socket);
    void run() override;
signals:

};

#endif // CLIENTTASK_H
