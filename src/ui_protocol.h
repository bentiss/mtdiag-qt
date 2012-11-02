/********************************************************************************
** Form generated from reading UI file 'protocol.ui'
**
** Created by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROTOCOL_H
#define UI_PROTOCOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_Protocol
{
public:
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *icon_tipswitch;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *icon_maxContacts;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *icon_slot;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLabel *icon_valid;
    QProgressBar *progressBar;

    void setupUi(QWidget *Form_Protocol)
    {
        if (Form_Protocol->objectName().isEmpty())
            Form_Protocol->setObjectName(QString::fromUtf8("Form_Protocol"));
        Form_Protocol->resize(527, 442);
        groupBox = new QGroupBox(Form_Protocol);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 30, 301, 381));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(groupBox);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 258, 343));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(scrollAreaWidgetContents);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(true);
        pushButton->setChecked(false);

        verticalLayout->addWidget(pushButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        icon_tipswitch = new QLabel(scrollAreaWidgetContents);
        icon_tipswitch->setObjectName(QString::fromUtf8("icon_tipswitch"));
        icon_tipswitch->setMaximumSize(QSize(40, 40));
        icon_tipswitch->setPixmap(QPixmap(QString::fromUtf8(":/icons/question.png")));
        icon_tipswitch->setScaledContents(true);

        horizontalLayout->addWidget(icon_tipswitch);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        icon_maxContacts = new QLabel(scrollAreaWidgetContents);
        icon_maxContacts->setObjectName(QString::fromUtf8("icon_maxContacts"));
        icon_maxContacts->setMaximumSize(QSize(40, 40));
        icon_maxContacts->setPixmap(QPixmap(QString::fromUtf8(":/icons/question.png")));
        icon_maxContacts->setScaledContents(true);

        horizontalLayout_2->addWidget(icon_maxContacts);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        icon_slot = new QLabel(scrollAreaWidgetContents);
        icon_slot->setObjectName(QString::fromUtf8("icon_slot"));
        icon_slot->setMaximumSize(QSize(40, 40));
        icon_slot->setPixmap(QPixmap(QString::fromUtf8(":/icons/question.png")));
        icon_slot->setScaledContents(true);

        horizontalLayout_3->addWidget(icon_slot);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        icon_valid = new QLabel(scrollAreaWidgetContents);
        icon_valid->setObjectName(QString::fromUtf8("icon_valid"));
        icon_valid->setMaximumSize(QSize(40, 40));
        icon_valid->setPixmap(QPixmap(QString::fromUtf8(":/icons/question.png")));
        icon_valid->setScaledContents(true);

        horizontalLayout_4->addWidget(icon_valid);


        verticalLayout->addLayout(horizontalLayout_4);

        progressBar = new QProgressBar(scrollAreaWidgetContents);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(Form_Protocol);
        QObject::connect(pushButton, SIGNAL(toggled(bool)), Form_Protocol, SLOT(autoDetect(bool)));

        QMetaObject::connectSlotsByName(Form_Protocol);
    } // setupUi

    void retranslateUi(QWidget *Form_Protocol)
    {
        Form_Protocol->setWindowTitle(QApplication::translate("Form_Protocol", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Form_Protocol", "Protocol", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form_Protocol", "Detect protocol", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form_Protocol", "TipSwitch", 0, QApplication::UnicodeUTF8));
        icon_tipswitch->setText(QString());
        label_3->setText(QApplication::translate("Form_Protocol", "max contacts", 0, QApplication::UnicodeUTF8));
        icon_maxContacts->setText(QString());
        label_2->setText(QApplication::translate("Form_Protocol", "Slot", 0, QApplication::UnicodeUTF8));
        icon_slot->setText(QString());
        label_4->setText(QApplication::translate("Form_Protocol", "Valid", 0, QApplication::UnicodeUTF8));
        icon_valid->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form_Protocol: public Ui_Form_Protocol {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROTOCOL_H
