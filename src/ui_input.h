/********************************************************************************
** Form generated from reading UI file 'input.ui'
**
** Created by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUT_H
#define UI_INPUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include "colorwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Form_Input
{
public:
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_node;
    QLabel *label;
    QLineEdit *lineEdit_driver;
    QLabel *label_4;
    QSpacerItem *verticalSpacer;
    ColorWidget *colorWidget;

    void setupUi(QWidget *Form_Input)
    {
        if (Form_Input->objectName().isEmpty())
            Form_Input->setObjectName(QString::fromUtf8("Form_Input"));
        Form_Input->resize(455, 561);
        groupBox = new QGroupBox(Form_Input);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(60, 60, 281, 261));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea_2 = new QScrollArea(groupBox);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 253, 223));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 2);

        lineEdit_node = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_node->setObjectName(QString::fromUtf8("lineEdit_node"));
        lineEdit_node->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit_node, 1, 2, 1, 1);

        label = new QLabel(scrollAreaWidgetContents_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 2, 0, 1, 1);

        lineEdit_driver = new QLineEdit(scrollAreaWidgetContents_2);
        lineEdit_driver->setObjectName(QString::fromUtf8("lineEdit_driver"));
        lineEdit_driver->setReadOnly(true);

        gridLayout_3->addWidget(lineEdit_driver, 2, 2, 1, 1);

        label_4 = new QLabel(scrollAreaWidgetContents_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 5, 2, 1, 1);

        colorWidget = new ColorWidget(scrollAreaWidgetContents_2);
        colorWidget->setObjectName(QString::fromUtf8("colorWidget"));

        gridLayout_3->addWidget(colorWidget, 4, 2, 1, 1);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea_2, 0, 0, 1, 1);


        retranslateUi(Form_Input);

        QMetaObject::connectSlotsByName(Form_Input);
    } // setupUi

    void retranslateUi(QWidget *Form_Input)
    {
        Form_Input->setWindowTitle(QApplication::translate("Form_Input", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Form_Input", "Informations", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form_Input", "Node:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form_Input", "Driver:", 0, QApplication::UnicodeUTF8));
        lineEdit_driver->setText(QString());
        label_4->setText(QApplication::translate("Form_Input", "Color:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form_Input: public Ui_Form_Input {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUT_H
