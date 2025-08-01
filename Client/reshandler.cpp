#include "client.h"
#include "index.h"
#include "index.h"
#include "reshandler.h"

#include <QMessageBox>
#include <QMessageBox>

ResHandler::ResHandler(PDU *pdu)
{
    this->pdu = pdu;
}

void ResHandler::regist()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&Client::getlnstance(),"注册","注册成功");
    } else {
        QMessageBox::information(&Client::getlnstance(),"注册","注册失败");
    }
}

void ResHandler::login()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().show();
        Client::getlnstance().hide();
    } else {
        QMessageBox::information(&Client::getlnstance(),"登录","登录失败");
    }
}

void ResHandler::findUser()
{

    char caName[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    int ret;
    memcpy(&ret,pdu->caData+32,sizeof(int));
    if(ret==-1){
        QMessageBox::information(&Index::getInstance(),"查找用户","该用户不存在");
    } else if(ret==1){
        int ret = QMessageBox::information(&Index::getInstance(),"查找用户","该用户在线","添加好友","取消");
        if(ret == 0){
            PDU* pdu = mkPDU();
            pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
            memcpy(pdu->caData,Client::getlnstance().m_strLoginName.toStdString().c_str(),32);
            memcpy(pdu->caData+32,caName,32);
            Client::getlnstance().sendMsg(pdu);
        }
    } else if(ret==0){
        QMessageBox::information(&Index::getInstance(),"查找用户","该用户不在线");
    } else if(ret==-2){
        QMessageBox::information(&Index::getInstance(),"查找用户","查找失败");
    }
}

void ResHandler::addFriend()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(!ret){
        QMessageBox::information(&Index::getInstance(),"添加好友","不满足添加好友条件");
    }
}

void ResHandler::addFriendResend()
{
    char caCurName[32]={'\0'};
    memcpy(caCurName,pdu->caData,32);
    int ret = QMessageBox::question(&Index::getInstance(),"添加好友",QString("是否同意 %1 的好友请求？").arg(caCurName));
    if(ret != QMessageBox::Yes){
        return ;
    }
    PDU* respdu=mkPDU();
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;
    memcpy(respdu->caData,pdu->caData,64);
    Client::getlnstance().sendMsg(respdu);
}

void ResHandler::addFriendAgree()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof (bool));
    if(ret){
            QMessageBox::information(&Index::getInstance(),"添加好友","添加好友成功");
            Index::getInstance().getFriend()->flushFriend();
        } else {
            QMessageBox::information(&Index::getInstance(),"添加好友","添加好友失败");
    }
}

void ResHandler::flushFriend()
{
    int iSize =pdu->uiMsgLen/32;//计算好友个数
    QStringList friendList;
    char caTmp[32] ={'\0'};
    for(int i=0;i<iSize;i++){
        memcpy(caTmp,pdu->caMsg+i*32,32);
        friendList.append(caTmp);
    }
    Index::getInstance().getFriend()->updateFriend_LW(friendList);
}

void ResHandler::chat()
{
    Chat* c = Index::getInstance().getFriend()->m_pChat;
    if(c->isHidden()){
        c->show();
    }
    char caChatName[32]={'\0'};
    memcpy(caChatName,pdu->caData,32);
    c->m_strChatName = caChatName;
    c->setWindowTitle(caChatName);
    c->updateShow_TE(pdu->caMsg);
}

void ResHandler::mkdir()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹成功");
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹失败");
    }
}

void ResHandler::flushFile()
{
    int iCount = pdu->uiMsgLen/sizeof(FileInfo);//文件数量
    qDebug()<<"iCount"<<iCount;
    QList<FileInfo*>pFileList;
    for(int i=0;i<iCount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg+i*sizeof(FileInfo),sizeof(FileInfo));
        pFileList.append(pFileInfo);
    }
    Index::getInstance().getFile()->updateFile_LW(pFileList);//调用函数作用是显示到文本框上
}

void ResHandler::moveFile()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        //QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹成功");
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(),"移动文件","移动文件失败");
    }
}

void ResHandler::uploadFileInit()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        //QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹成功");
        Index::getInstance().getFile()->uploadFile();
    } else {
        QMessageBox::information(&Index::getInstance(),"上传文件","上传文件失败");
    }
}

void ResHandler::shareFile()
{
    QMessageBox::information(&Index::getInstance(),"分享文件","文件已分享");
}

void ResHandler::shareFileResend()//分享文件，接收分享的用户处理的函数
{
    QString strSharePath = QString(pdu->caMsg);//将分享给我的文件路径提取出来
    int index = strSharePath.lastIndexOf('/');//找到最后一个 / 的下标
    QString strFileName = strSharePath.right(strSharePath.size()-index-1);//这个 / 的右边是文件名字
    int ret = QMessageBox::question(&Index::getInstance(),"分享文件",QString("%1 分享文件： %2\n 是否同意接收？").arg(pdu->caData).arg(strFileName));//弹窗询问用户是否接收
    if(ret != QMessageBox::Yes){//如果拒绝接收 直接return
        return;
    }
    PDU* respdu = mkPDU(pdu->uiMsgLen);//我同意接收之后发给服务器复制。因为本来pdu里面camsg存的是文件路径。这里长度直接用
    respdu->uiMsgType=ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST;
    memcpy(respdu->caData,Client::getlnstance().m_strLoginName.toStdString().c_str(),32);//将我接收者的用户名存到cadata里面
    memcpy(respdu->caMsg,pdu->caMsg,pdu->uiMsgLen);//直接将pdu的camsg存的文件路径复制过来
    Client::getlnstance().sendMsg(respdu);//发给服务器
}

void ResHandler::shareFileAgree()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getInstance().getFile()->flushFile();
    } else {
        QMessageBox::information(&Index::getInstance(),"分享文件","分享文件失败");
    }
}
