#ifndef RESHANDLER_H
#define RESHANDLER_H

#include "protocol.h"



class ResHandler
{
public:
    ResHandler(PDU* pdu);
    PDU* pdu;
    void regist();
    void login();
    void findUser();
    void addFriend();
    void addFriendResend();
    void addFriendAgree();
    void flushFriend();
    void chat();
    void mkdir();
    void flushFile();
    void moveFile();
    void uploadFileInit();
    void shareFile();
    void shareFileResend();
    void shareFileAgree();
};

#endif // RESHANDLER_H
