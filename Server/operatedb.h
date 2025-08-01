#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include<QSqlDatabase>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase m_db;//数据库对象
    static OperateDB& getInstance();
    void connect();
    ~OperateDB();
    bool handleRegist(const char* name,const char* pwd);
    bool logine(const char* name,const char* pwd);
    void handleOffline(const char* name);
    int handleSearchUser(const char* name);
    bool handleAddFriend(const char* curName,const char* tarName);
    bool handleAddFriendAgree(const char* curName,const char* tarName);
    QStringList handleFlushFriend(const char* curName);
signals:

private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB(const OperateDB& instance)=delete;
    OperateDB& operator=(const OperateDB&)=delete;
};

#endif // OPERATEDB_H
