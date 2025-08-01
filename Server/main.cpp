#include "server.h"

#include <QApplication>
#include "operatedb.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperateDB::getInstance().connect();//调用连接数据库的函数
    Server::getInstance();
//    w.show();
    return a.exec();
}
