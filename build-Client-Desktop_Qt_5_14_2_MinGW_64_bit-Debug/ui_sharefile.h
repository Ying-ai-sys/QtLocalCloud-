/********************************************************************************
** Form generated from reading UI file 'sharefile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREFILE_H
#define UI_SHAREFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sharefile
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *allSelected_PB;
    QPushButton *cancelSelected_PB;
    QSpacerItem *verticalSpacer;
    QPushButton *ok_PB;

    void setupUi(QWidget *Sharefile)
    {
        if (Sharefile->objectName().isEmpty())
            Sharefile->setObjectName(QString::fromUtf8("Sharefile"));
        Sharefile->resize(400, 300);
        horizontalLayout = new QHBoxLayout(Sharefile);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(Sharefile);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        allSelected_PB = new QPushButton(Sharefile);
        allSelected_PB->setObjectName(QString::fromUtf8("allSelected_PB"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        allSelected_PB->setFont(font);

        verticalLayout->addWidget(allSelected_PB);

        cancelSelected_PB = new QPushButton(Sharefile);
        cancelSelected_PB->setObjectName(QString::fromUtf8("cancelSelected_PB"));
        cancelSelected_PB->setFont(font);

        verticalLayout->addWidget(cancelSelected_PB);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ok_PB = new QPushButton(Sharefile);
        ok_PB->setObjectName(QString::fromUtf8("ok_PB"));
        ok_PB->setFont(font);

        verticalLayout->addWidget(ok_PB);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Sharefile);

        QMetaObject::connectSlotsByName(Sharefile);
    } // setupUi

    void retranslateUi(QWidget *Sharefile)
    {
        Sharefile->setWindowTitle(QCoreApplication::translate("Sharefile", "Form", nullptr));
        allSelected_PB->setText(QCoreApplication::translate("Sharefile", "\345\205\250\351\200\211", nullptr));
        cancelSelected_PB->setText(QCoreApplication::translate("Sharefile", "\345\217\226\346\266\210\351\200\211\344\270\255", nullptr));
        ok_PB->setText(QCoreApplication::translate("Sharefile", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Sharefile: public Ui_Sharefile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREFILE_H
