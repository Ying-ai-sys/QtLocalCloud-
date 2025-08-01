#include "operatedb.h"
#include "QDebug"
#include "QSqlError"

#include <QSqlQuery>
OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()//处理连接的
{
    m_db.setHostName("localhost");//主机名，本机是localhost
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("mydb0507");
    if(m_db.open()){//打开数据库
        qDebug()<<"数据库连接成功";
    }else{
        qDebug()<<"数据库连接失败"<<m_db.lastError().text();//打开失败原因
    }
}

OperateDB::~OperateDB()//析构函数
{
    m_db.close();//关闭m_db
}
//在数据库操作类中创建一个处理注册的函数，参数为用户名和密码，返回值为bool
bool OperateDB::handleRegist(const char *name, const char *pwd)
{
    if(name==NULL||pwd==NULL){
        return false;
    }
//构建查找用户是否存在的sql语句
    QString sql = QString("select * from user_info where name='%1'").arg(name);
    qDebug()<<"handleRegist sql：查询用户是否存在"<<sql;
    QSqlQuery q;//执行sql语句用到的
    if(!q.exec(sql) || q.next()){
        return false;
    }
//添加一条用户记录的sql语句
    sql=QString("insert into user_info(name,pwd) values('%1','%2')").arg(name).arg(pwd);
    qDebug()<<"sql：添加用户"<<sql;
    return q.exec(sql);
}

bool OperateDB::logine(const char *name, const char *pwd)
{
    if(name==NULL||pwd==NULL){
        return false;
    }
//构建查找用户是否存在的sql语句
    QString sql = QString("select * from user_info where name ='%1' and pwd = '%2'").arg(name).arg(pwd);
    qDebug()<<"logine sql：查询用户密码是否存在"<<sql;
    QSqlQuery q;//执行sql语句用到的
    if(!q.exec(sql) || !q.next()){
        return false;
    }
    sql=QString("update user_info set online=1 where name='%1' and pwd='%2'").arg(name).arg(pwd);
    qDebug()<<"sql：设置登录状态"<<sql;
    return q.exec(sql);
}

void OperateDB::handleOffline(const char *name)
{
    if(name==NULL){
        return ;
    }
    QSqlQuery q;//执行sql语句用到的
    QString sql=QString("update user_info set online=0 where name='%1'").arg(name);
    qDebug()<<"handleOffline sql：更新用户下线"<<sql;
    q.exec(sql);
}

int OperateDB::handleSearchUser(const char *name)
{
    if(name==NULL){
        return -2;
    }
    QSqlQuery q;
    QString sql = QString("select online from user_info where name ='%1'").arg(name);
    qDebug()<<"handleSearchUser sql：查询用户是否在线"<<sql;
    if(!q.exec(sql)){
        return -2;
    }
    if(q.next()){
        return q.value(0).toInt();
    } else {
        return -1;
    }
}

bool OperateDB::handleAddFriend(const char *curName, const char *tarName)
{
    if(curName==NULL||tarName==NULL){
        return false;
    }
    QString sql=QString(R"(
       select * from friend
       where
       (
       user_id=(select id from user_info where name='%1')
       and
       friend_id=(select id from user_info where name='%2')
       )
       or
       (
       user_id=(select id from user_info where name='%2')
       and
       friend_id=(select id from user_info where name='%1')
       )
                        )").arg(curName).arg(tarName);
    qDebug()<<"handleAddFriend sql：查询是否是好友"<<sql;
    QSqlQuery q;
    if(!q.exec(sql)){
        return false;
    }
    if(q.next()){
        return false;
    }
    sql=QString("select online from user_info where name ='%1'").arg(tarName);
    qDebug()<<"handleAddFriend sql：查询用户是否在线"<<sql;
    if(!q.exec(sql)){
        return false;
    }
    if(q.next() && q.value(0).toInt()){
        return true;
    }else {
        return false;
    }
}

bool OperateDB::handleAddFriendAgree(const char *curName, const char *tarName)
{
    if(curName==NULL||tarName==NULL){
        return false;
    }
    QString sql=QString(R"(
       insert into friend(user_id,friend_id)
       select u1.id,u2.id
       from user_info u1,user_info u2
       where u1.name = '%1' and u2.name = '%2'
                        )").arg(curName).arg(tarName);
    qDebug()<<"handleAddFriendAgree sql：添加一条好友记录"<<sql;
    QSqlQuery q;
    return q.exec(sql);
}

QStringList OperateDB::handleFlushFriend(const char *curName)
{
    QStringList result;
    if(curName == NULL){
        return result;
    }
    QString sql=QString(R"(
      select name from user_info
      where online =1 and id in
      (
      select friend_id from friend where user_id =
      (select id from user_info where name = '%1')
       union
       select user_id from friend where friend_id =
       (select id from user_info where name = '%1')
       )
                        )").arg(curName);
    QSqlQuery q;
    if(!q.exec(sql)){
        return {};
    }
    qDebug()<<"handleFlushFriend sql：添加一条好友记录"<<sql;
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");//初始化m_db对象
}
