#ifndef FILE_H
#define FILE_H

#include "protocol.h"
#include "sharefile.h"

#include <QListWidget>
#include <QWidget>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    QString m_strUserPath;//限制路径
    QString m_strCurPath;//当前路径
    QString m_strMoveFileName;//移动的文件名
    QString m_strShareFileName;//分享的文件名
    Sharefile* m_pShareFile;
    QString m_strMoveFilePath;//移动的完整路径
    QString m_strUploadFilePath;//上传文件，选择的文件路径
    QList<FileInfo*> m_pFileInfoList;
    void flushFile();//封装的刷新文件的函数
    //void uploadFile();//上传文件的函数
    void uploadFile();//新的上传文件的函数
    void updateFile_LW(QList<FileInfo*>pFileList);//刷新列表框的函数
public slots:

    void errorSlot(QString error);//处理报错的槽函数

private slots:
    void on_mkdir_PB_clicked();

    void on_flush_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_move_PB_clicked();

    void on_upload_PB_clicked();

    void on_share_PB_clicked();



private:
    Ui::File *ui;
};

#endif // FILE_H
