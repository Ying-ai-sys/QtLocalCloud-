#ifndef UPLOADER_H
#define UPLOADER_H

#include "protocol.h"

#include <QObject>

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = nullptr);
    void start();//移交的函数
    QString m_strUploadFilePath;//上传文件的路径 设置成成员函数
    Uploader(QString strFilePath);//为了给上面的路径初始化，新添加一个构造函数
public slots:
    void uploadFile();//处理上传的函数,把它变成槽函数因为要关联
signals:
    void errorSignal(QString error);//报错弹窗的信号，传入的参数为报错信息
    void sendPDU(PDU* pdu);//发送的信号，参数传入发送的PDU
    void finished();//定义结束的信号，全部上传完线程结束
};

#endif // UPLOADER_H
