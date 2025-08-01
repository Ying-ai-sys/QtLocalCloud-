#ifndef REQHANDLER_H
#define REQHANDLER_H

#include "protocol.h"

#include <QFile>
#include <QString>



class ReqHandler
{
public:
    ReqHandler();
    PDU* pdu;
    qint64* m_iUploadFileSize;
    QFile m_fUploadFile;
    qint64* m_iUploadReceived;
    PDU* regist();
    PDU* login(QString& strLoginName);
    PDU* findUser();
    PDU* addFriend();
    PDU* addFriendAgree();
    PDU* flushFriend();
    PDU* chat();
    PDU* mkdir();
    PDU* flushFile();
    PDU* moveFile();
    PDU* uploadFile();
    PDU* uploadFileData();
    PDU* ShareFile();
    PDU* ShareFileAgree();
};

#endif // REQHANDLER_H
