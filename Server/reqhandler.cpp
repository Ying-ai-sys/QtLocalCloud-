#include "mytcpserver.h"
#include "operatedb.h"
#include "reqhandler.h"
#include "server.h"
#include "server.h"
#include <QDebug>
#include <QDir>
ReqHandler::ReqHandler()
{
}

PDU *ReqHandler::regist()
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
//从caData 中取出用户名和密码
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
//调用处理注册的函数，得到结果。数据库中处理注册的函数
    bool ret = OperateDB::getInstance().handleRegist(caName,caPwd);
    if(ret){
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(caName));
        //每注册一个用户在目录下创建一个文件夹，以该用户名命名
    }
    qDebug()<<"regist:ret"<<ret;
//创建一个pdu，将得到的结果放到pdu里返回给客户端
    PDU* respdu=mkPDU();//不传参默认为0，代表没用柔性数组
//类型为注册响应
    respdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;
//将结果数据放到caData里面
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *ReqHandler::login(QString &strLoginName)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().logine(caName,caPwd);
    qDebug()<<"login:ret"<<ret;
    if(ret){
        strLoginName=caName;
    }
//创建一个pdu，将得到的结果放到pdu里返回给客户端
    PDU* respdu=mkPDU();//不传参默认为0，代表没用柔性数组
//类型为登录响应
    respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;
//将结果数据放到caData里面
    memcpy(respdu->caData,&ret,sizeof(bool));
//tcpsocket 传给客户端
    return respdu;
}

PDU *ReqHandler::findUser()
{
    char caName[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    int ret = OperateDB::getInstance().handleSearchUser(caName);
    qDebug()<<"findUser:ret"<<ret;
    PDU* respdu =mkPDU();
    respdu->uiMsgType=ENUM_MSG_TYPE_FIND_USER_RESPOND;
    memcpy(respdu->caData,caName,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}

PDU *ReqHandler::addFriend()
{
    char caCurName[32]={'\0'};
    char caTarName[32]={'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleAddFriend(caCurName,caTarName);
    qDebug()<<"addFriend:ret"<<ret;
    if(ret == true){
        pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESEND;
        MyTcpServer::getInstance().resend(caTarName,pdu);
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;

}

PDU *ReqHandler::addFriendAgree()
{
    char caCurName[32] ={'\0'};
    char caTarName[32] ={'\0'};
    memcpy(caCurName,pdu->caData,32);
    memcpy(caTarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleAddFriendAgree(caCurName,caTarName);
    qDebug()<<"addFriendAgree:ret"<<ret;
    PDU* respdu = mkPDU();
    respdu->uiMsgType= ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    MyTcpServer::getInstance().resend(caCurName,respdu);
    return respdu;
}

PDU *ReqHandler::flushFriend()
{
    char caCurName[32]={'\0'};
    memcpy(caCurName,pdu->caData,32);
    QStringList result = OperateDB::getInstance().handleFlushFriend(caCurName);
    qDebug()<<result.size();
    PDU* respdu =mkPDU(result.size()*32);
    respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
    for(int i=0;i<result.size();i++){
        qDebug()<<"好友名"<<result.at(i);
        memcpy(respdu->caMsg+i*32,result.at(i).toStdString().c_str(),32);
    }
    return respdu;
}

PDU *ReqHandler::chat()
{
    char caTarName[32] ={'\0'};
    memcpy(caTarName,pdu->caData+32,32);
    MyTcpServer::getInstance().resend(caTarName,pdu);//pdu转发给tar
    return NULL;
}

PDU *ReqHandler::mkdir()
{
    char caNewDir[32] ={'\0'};
    memcpy(caNewDir,pdu->caData,32);
    QString strCurPath = pdu->caMsg;
    QDir dir;
    bool ret = true;
    if(!dir.exists(strCurPath)){
        ret = false;
        qDebug()<<111;
    }else{
        QString strNewPath = QString("%1/%2").arg(strCurPath).arg(caNewDir);
        if(dir.exists(strNewPath)){
            ret = false;
            qDebug()<<222;
        }else{
            ret = dir.mkdir(strNewPath);
        }
    }
    qDebug()<<"创建文件夹的ret"<<ret;
    PDU* respdu = mkPDU();
    respdu->uiMsgType= ENUM_MSG_TYPE_MKDIR_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *ReqHandler::flushFile()
{
    QString strCurPath = pdu->caMsg;
    QDir dir(strCurPath);
    QFileInfoList fileInfoList = dir.entryInfoList();//用entryInfoList获取指定目录下的所有子目录名和文件名
    int iFileCount = fileInfoList.size();
    PDU* respdu = mkPDU(sizeof(FileInfo)*(iFileCount-2));//申请的空间，用文件的个数乘上结构体大小
    qDebug()<<"刷新文件文件的数量"<<fileInfoList.size();
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
    for (int i=0,j=0;i<fileInfoList.size();i++){      //指针偏移所以＋i
        if(fileInfoList.at(i).fileName()=="."||fileInfoList.at(i).fileName()==".."){
            continue;
        }
        FileInfo* pFileInfo = (FileInfo*)respdu->caMsg + j++;//将caMsg 转换为结构体类型的
        memcpy(pFileInfo->caName,fileInfoList.at(i).fileName().toStdString().c_str(),32);
        if(fileInfoList.at(i).isDir()){//判断是否为目录
            pFileInfo->iType=0;
        }else{//否则是文件
            pFileInfo->iType=1;
        }
    }
    return respdu;
}

PDU *ReqHandler::moveFile()
{
    int iSrcLen =0;
    int iTarLen =0;
    memcpy(&iSrcLen,pdu->caData,sizeof (int));
    memcpy(&iTarLen,pdu->caData+32,sizeof(int));
    char* pSrcPath = new char[iSrcLen+1];
    char* pTarPath = new char[iTarLen+1];
    memset(pSrcPath,'\0',iSrcLen+1);
    memset(pTarPath,'\0',iTarLen+1);
    memcpy(pSrcPath,pdu->caMsg,iSrcLen);
    memcpy(pTarPath,pdu->caMsg+iSrcLen,iTarLen);
    bool ret = QFile::rename(pSrcPath,pTarPath);
    PDU* respdu = mkPDU();
    respdu->uiMsgType=ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(ret));
    delete [] pSrcPath;
    delete [] pTarPath;
    pSrcPath =NULL;
    pTarPath = NULL;
    return respdu;
}

PDU *ReqHandler::uploadFile()
{
    char caFileName[32] = {'\0'};
    memcpy(caFileName,pdu->caData,32);
    m_iUploadFileSize = 0;
    memcpy(&m_iUploadFileSize,pdu->caData+32,sizeof(qint64));
    m_fUploadFile.setFileName(QString("%1/%2").arg(pdu->caMsg).arg(caFileName));
    bool ret = m_fUploadFile.open(QIODevice::WriteOnly);//创建文件,用只写模式
    m_iUploadReceived = 0;
    PDU* respdu = mkPDU();
    respdu->uiMsgType = ENUM_MSG_TYPE_UOLOAD_FILE_INIT_RESPOND;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}

PDU *ReqHandler::uploadFileData()
{
    m_fUploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    m_iUploadReceived += pdu->uiMsgLen;
    if(m_iUploadReceived < m_iUploadFileSize){
        return NULL;//表示还没读取完
    }
    m_fUploadFile.close();
    PDU* respdu = mkPDU();
    respdu->uiMsgType = ENUM_MSG_TYPE_UOLOAD_FILE_DATA_RESPOND;
    return respdu;
}

PDU *ReqHandler::ShareFile()
{
    char caCurName[32]={'\0'};
    int iFriendNum =0;
    memcpy(caCurName,pdu->caData,32);
    memcpy(&iFriendNum,pdu->caData+32,sizeof(int));
    qDebug()<<"ShareFile iFriendNum"<<iFriendNum;
    PDU* resendpdu = mkPDU(pdu->uiMsgLen-iFriendNum*32);//pdu->uiMsgLen-iFriendNum*32 总长度-选中好友的长度=路径的长度
    resendpdu->uiMsgType = pdu->uiMsgType;//类型为请求
    memcpy(resendpdu->caData,caCurName,32);//发送者的名字
    memcpy(resendpdu->caMsg,pdu->caMsg+iFriendNum*32,pdu->uiMsgLen-iFriendNum*32);//想要分享的文件路径
    char caRecvName[32]={'\0'};
    for(int i=0;i<iFriendNum;i++){
        memcpy(caRecvName,pdu->caMsg+i*32,32);//把每一个选中的分享好友 提取出来
        MyTcpServer::getInstance().resend(caRecvName,resendpdu);//转发 依次转发
    }
    free(resendpdu); //转发函数里面没有释放，在这里释放
    resendpdu = NULL;
    PDU* respdu = mkPDU();
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
    return respdu;
}

PDU *ReqHandler::ShareFileAgree()
{
    QString strShareFilePath = pdu->caMsg;
    int index = strShareFilePath.lastIndexOf('/');
    QString strFileName = strShareFilePath.right(strShareFilePath.size()-index-1);
    QString strRecvPath = QString("%1/%2/%3").arg(Server::getInstance().m_strRootPath).arg(pdu->caData).arg(strFileName);
    QFileInfo fileInfo(strShareFilePath);
    bool ret = true;
    if(fileInfo.isFile()){
        ret = QFile::copy(strShareFilePath,strRecvPath);

    }else{
        //复制目录
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof (bool));
    return respdu;
}
