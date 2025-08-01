#include "file.h"
#include "ui_file.h"
#include "client.h"
#include "uploader.h"
#include "client.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2").arg(Client::getlnstance().m_strRootPath).arg(Client::getlnstance().m_strLoginName);
    m_strCurPath = m_strUserPath;//初始化
    flushFile();
    m_pShareFile = new Sharefile;
}

File::~File()
{
    delete ui;
    delete m_pShareFile;
}

void File::flushFile()
{
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getlnstance().sendMsg(pdu);
}

//void File::uploadFile()
//{
//    //创建QFile对象并打开
//    QFile file(m_strUploadFilePath);
//    if(!file.open(QIODevice::ReadOnly)){
//        QMessageBox::warning(this,"上传文件","上传文件失败");
//        return;
//    }
//    //构建PDU，循环读取文件内容
//    PDU* pdu = mkPDU(4096);
//    pdu->uiMsgType = ENUM_MSG_TYPE_UOLOAD_FILE_DATA_REQUEST;
//    while(1){
//        qint64 ret = file.read(pdu->caMsg,4096);//参数一是读取到哪
//        if(ret == 0){
//            break;
//        }
//        if(ret < 0){
//            QMessageBox::warning(this,"上传文件","上传文件失败");
//            break;
//        }
//        pdu->uiMsgLen=ret;
//        pdu->uiPDULen=ret + sizeof(PDU);
//        //因为这个pdu只创建了一次，需要循环发送，不能直接调用发送函数，因为里面直接将pdu释放掉了
//        Client::getlnstance(). m_tcpSocket.write((char*)pdu,pdu->uiPDULen);//客户端和服务器通信，发送
//        qDebug()<<"uploadFile pdu->uiPDULen"<<pdu->uiPDULen
//                <<"pdu->uiMsgLen"<<pdu->uiMsgLen
//                <<"pdu->uiMsgType"<<pdu->uiMsgType
//                <<"pdu->caData"<<pdu->caData
//                <<"pdu->caData+32"<<pdu->caData+32
//                <<"pdu->caMsg"<<pdu->caMsg;
//    }
//}

void File::updateFile_LW(QList<FileInfo *> pFileList)
{
    ui->listWidget->clear();
    foreach(FileInfo* pFileInfo,m_pFileInfoList){//每一次更新目录前，先把之前目录下存下来的所有文件信息释放掉
        delete pFileInfo;
    }
    m_pFileInfoList = pFileList;//都有什么文件信息存下来
    foreach (FileInfo* pFileInfo,pFileList){
        QListWidgetItem* pItem = new QListWidgetItem;//构建列表框元素
        if(pFileInfo->iType == 0){
            pItem->setIcon(QIcon(QPixmap(":/wenjianjia.png")));
        }else if(pFileInfo->iType == 1){
            pItem->setIcon(QIcon(QPixmap(":/wenjian.png")));
        }
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);//将列表框元素填到显示的框里
    }
}

void File::on_mkdir_PB_clicked()
{
    //弹窗，让用户输入创建的文件夹名
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新建文件夹名：");
    //限制文件夹名的长度,为空或者是大于32都弹窗非法
    if(strNewDir.isEmpty()||strNewDir.toStdString().size() > 32){
        QMessageBox::information(this,"新建文件夹","文件夹名长度非法");
        return ;
    }
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getlnstance().sendMsg(pdu);
}

void File::on_flush_PB_clicked()
{
    flushFile();
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strDirName = item->text();
    foreach(FileInfo* pFileInfo,m_pFileInfoList){
        if(pFileInfo->caName == strDirName && pFileInfo->iType !=0){
            QMessageBox::warning(this,"提示","选中的不是文件夹");
            return ;
        }
    }
    m_strCurPath=m_strCurPath+'/'+strDirName;
    flushFile();
}

void File::on_return_PB_clicked()
{
    if(m_strCurPath == m_strUserPath){
        //QMessageBox::warning(this,"返回","返回失败：已在顶层目录");
        return ;
    }
    //找最后一个/，从/开始删除
    int index = m_strCurPath.lastIndexOf('/');//找最后一个/，返回值是下标
    m_strCurPath.remove(index,m_strCurPath.size()-index);//删除的长度用总长度减去最后一个/的下标
    flushFile();
}

void File::on_move_PB_clicked()
{
    //点击移动按钮
    if(ui->move_PB->text() == "移动文件"){
        //用当前项获取想要移动的文件名
        QListWidgetItem* pItem = ui->listWidget->currentItem();
        //如果这个文件名为空，证明我还没选，给个弹窗告知用户选择
        if(pItem == NULL){
            QMessageBox::warning(this,"移动文件","请选择要移动的文件");
        }
        //如果已经选择了要移动的目录，给个弹窗提示用户选择要将文件移动到的目录
        QMessageBox::information(this,"移动文件","请选择移动到的目录");
        //将按钮名变为确认/取消
        ui->move_PB->setText("确认/取消");
        //用成员变量将要移动的名和完整路径存下来
        m_strMoveFileName = pItem->text();
        m_strMoveFilePath = m_strCurPath +'/' +m_strMoveFileName;
        //第一次点击移动文件结束，等待点击下次的确认/取消
        return;
    }
    //我现在已经选择了我要移动到的目录，就是当前目录，将当前目录和移动的文件名拼接在一起就是我要移动到的完整目录
    QString strTarPath =  m_strCurPath +'/' +m_strMoveFileName;
    //将按钮名改回来
    ui->move_PB->setText("移动文件");
    //弹窗询问用户是否移动到这个目录下
    int ret = QMessageBox::information(this,"移动文件",QString("是否移动到 %1 ?").arg(m_strCurPath),"确认","取消");
    //如果点击的是取消就直接结束
    if(ret !=0){ //0是确认
        return;
    }
    //如果点击的是确认就将移动的文件路径长度和目标路径长度存到pdu的cadata里面
    int iSrcLen = m_strMoveFilePath.toStdString().size();
    int iTarLen = strTarPath.toStdString().size();
    PDU* pdu = mkPDU(iSrcLen + iTarLen +1);
    pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    memcpy(pdu->caData,&iSrcLen,32);
    memcpy(pdu->caData+32,&iTarLen,32);
    //将 移动文件的路径和目标路径存到camsg里面
    memcpy(pdu->caMsg,m_strMoveFilePath.toStdString().c_str(),iSrcLen);
    memcpy(pdu->caMsg+iSrcLen,strTarPath.toStdString().c_str(),iTarLen);
    //发送给服务器
    Client::getlnstance().sendMsg(pdu);
}

void File::on_upload_PB_clicked()
{
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    qDebug()<<"m_strUploadFilePath"<<m_strUploadFilePath;
    if(m_strUploadFilePath.isEmpty()){
        return;
    }
    int index = m_strUploadFilePath.lastIndexOf('/');
    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size()-index-1);
    QFile file(m_strUploadFilePath);
    qint64 iFileSize= file.size();
    PDU* pdu =mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UOLOAD_FILE_INIT_REQUEST;
    memcpy(pdu->caData,strFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&iFileSize,32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getlnstance().sendMsg(pdu);
}

void File::on_share_PB_clicked()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    //如果这个文件名为空，证明我还没选，给个弹窗告知用户选择
    if(pItem == NULL){
        QMessageBox::warning(this,"分享文件","请选择要分享的文件");
        return;
    }
    m_strShareFileName = m_strCurPath +'/'+ pItem->text();//获取我点击想要分享的文件的名字，存到成员变量里
    m_pShareFile->updataFriend_LW();
    if(m_pShareFile->isHidden()){//如果我选择分享好友的弹窗没显示，给显示出来
        m_pShareFile->show();
    }
}
void File::uploadFile()//在之前的上传文件里 new一个新创建的类对象用来专门处理上传函数的类对象
{
    Uploader* uploader = new Uploader(m_strUploadFilePath);
    connect(uploader, &Uploader::errorSignal, this, &File::errorSlot);
    connect(uploader, &Uploader::sendPDU, &Client::getlnstance(), &Client::sendMsg);
    uploader->start();//调用新类对象的移交函数
}

void File::errorSlot(QString error)
{
    QMessageBox::information(this, "提示", error);
}
