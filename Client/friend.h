#ifndef FRIEND_H
#define FRIEND_H

#include "chat.h"

#include <QListWidget>
#include <QWidget>

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    void flushFriend();//刷新好友的一个函数，方便以后调用
    void updateFriend_LW(QStringList friendList);
    Chat* m_pChat;
    QListWidget* getFriend_LW();

private slots:
    void on_findUser_PB_clicked();

    void on_flush_PB_clicked();

    void on_chat_PB_clicked();

private:
    Ui::Friend *ui;
};

#endif // FRIEND_H
