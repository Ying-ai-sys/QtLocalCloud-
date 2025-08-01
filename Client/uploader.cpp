#include "protocol.h"
#include "uploader.h"

#include <QFile>
#include <QThread>

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

void Uploader::start()//主要处理移交，将当前类对象移交给线程
{
    QThread* thread = new QThread;//创建一个QThread对象 创建线程
    this->moveToThread(thread);//将当前类对象移交进去，移交给线程
    connect(thread, &QThread::started, this, &Uploader::uploadFile);//started线程开始的信号
    connect(this, &Uploader::finished, thread, &QThread::quit);//当前类结束的信号，线程处理退出
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);//线程完成的信号（两个完成的信号不一样），线程处理释放移除（延迟移除）
    thread->start();//启动线程
}

Uploader::Uploader(QString strFilePath)//创建的构造函数，给上传文件路径的成员函数初始化的
{
    m_strUploadFilePath = strFilePath;
}

void Uploader::uploadFile()//上传文件的内容
{
    //创建QFile对象并打开
    QFile file(m_strUploadFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit errorSignal("上传文件失败");//弹窗是控件，控件不能放在这个类里，所以也用到信号
        emit finished();//return都是线程结束都要调用结束的信号
        return;
    }
    //构建pdu，循环读取文件内容并发送
    while (true) {
        PDU* pdu = mkPDU(4096);//pdu不能放在循环外面了。因为发送一个pdu，需要用信号通知主线程发送，不在一个线程里，主线程不会立刻发完，有可能主线程还没发送完这边已经将pdu改变，所以要用不同的pdu
        pdu->uiMsgType = ENUM_MSG_TYPE_UOLOAD_FILE_DATA_REQUEST;
        qint64 ret = file.read(pdu->caMsg, 4096);
        if (ret == 0) {
            break;
        }
        if (ret < 0) {
            emit errorSignal("读取文件失败");//弹窗是控件，控件不能放在这个类里，所以也用到信号
            break;
        }
        pdu->uiMsgLen = ret;
        pdu->uiPDULen = ret + sizeof(PDU);
        emit sendPDU(pdu);//socket发送不能放在这个类里，要放在主线程里，所以这里用信号
    }
    file.close();//打开需要上传的文件之后需要关闭
    emit finished();//全部上传完 需要发出结束信号
}
