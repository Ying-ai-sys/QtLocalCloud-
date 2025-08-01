/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QGridLayout *gridLayout;
    QPushButton *mkdir_PB;
    QPushButton *return_PB;
    QPushButton *flush_PB;
    QPushButton *rename_PB;
    QPushButton *delfile_PB;
    QPushButton *share_PB;
    QPushButton *deldir_PB;
    QPushButton *upload_PB;
    QPushButton *move_PB;
    QPushButton *download_PB;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(678, 450);
        horizontalLayout = new QHBoxLayout(File);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(File);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mkdir_PB = new QPushButton(File);
        mkdir_PB->setObjectName(QString::fromUtf8("mkdir_PB"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        mkdir_PB->setFont(font);

        gridLayout->addWidget(mkdir_PB, 0, 0, 1, 1);

        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        return_PB->setFont(font);

        gridLayout->addWidget(return_PB, 0, 1, 1, 1);

        flush_PB = new QPushButton(File);
        flush_PB->setObjectName(QString::fromUtf8("flush_PB"));
        flush_PB->setFont(font);

        gridLayout->addWidget(flush_PB, 1, 0, 1, 1);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        rename_PB->setFont(font);

        gridLayout->addWidget(rename_PB, 1, 1, 1, 1);

        delfile_PB = new QPushButton(File);
        delfile_PB->setObjectName(QString::fromUtf8("delfile_PB"));
        delfile_PB->setFont(font);

        gridLayout->addWidget(delfile_PB, 2, 0, 1, 1);

        share_PB = new QPushButton(File);
        share_PB->setObjectName(QString::fromUtf8("share_PB"));
        share_PB->setFont(font);

        gridLayout->addWidget(share_PB, 2, 1, 1, 1);

        deldir_PB = new QPushButton(File);
        deldir_PB->setObjectName(QString::fromUtf8("deldir_PB"));
        deldir_PB->setFont(font);

        gridLayout->addWidget(deldir_PB, 3, 0, 1, 1);

        upload_PB = new QPushButton(File);
        upload_PB->setObjectName(QString::fromUtf8("upload_PB"));
        upload_PB->setFont(font);

        gridLayout->addWidget(upload_PB, 3, 1, 1, 1);

        move_PB = new QPushButton(File);
        move_PB->setObjectName(QString::fromUtf8("move_PB"));
        move_PB->setFont(font);

        gridLayout->addWidget(move_PB, 4, 0, 1, 1);

        download_PB = new QPushButton(File);
        download_PB->setObjectName(QString::fromUtf8("download_PB"));
        download_PB->setFont(font);

        gridLayout->addWidget(download_PB, 4, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        mkdir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        flush_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        delfile_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        share_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        deldir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        upload_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        move_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        download_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
