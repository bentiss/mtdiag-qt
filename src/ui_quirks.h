/********************************************************************************
** Form generated from reading UI file 'quirks.ui'
**
** Created by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUIRKS_H
#define UI_QUIRKS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_Quirks
{
public:
    QDockWidget *dockWidget_quirks;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_Valid;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *radioButton_NOT_SEEN_MEANS_UP;
    QRadioButton *radioButton_ALWAYS_VALID;
    QRadioButton *radioButton_VALID_IS_INRANGE;
    QRadioButton *radioButton_VALID_IS_CONFIDENCE;
    QGroupBox *groupBox_Slot;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_calculated;
    QRadioButton *radioButton_SLOT_IS_CONTACTID;
    QRadioButton *radioButton_SLOT_IS_CONTACTNUMBER;
    QRadioButton *radioButton_SLOT_IS_CONTACTID_MINUS_ONE;
    QFrame *line;
    QLabel *label;
    QRadioButton *radioButton_CYPRESS;
    QGroupBox *groupBox_vendor;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_WIN_8_CERTIFIED;
    QCheckBox *checkBox_NO_AREA;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Form_Quirks)
    {
        if (Form_Quirks->objectName().isEmpty())
            Form_Quirks->setObjectName(QString::fromUtf8("Form_Quirks"));
        Form_Quirks->resize(324, 513);
        dockWidget_quirks = new QDockWidget(Form_Quirks);
        dockWidget_quirks->setObjectName(QString::fromUtf8("dockWidget_quirks"));
        dockWidget_quirks->setGeometry(QRect(9, 9, 225, 451));
        dockWidget_quirks->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout = new QGridLayout(dockWidgetContents_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(dockWidgetContents_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, -154, 201, 574));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_Valid = new QGroupBox(scrollAreaWidgetContents);
        groupBox_Valid->setObjectName(QString::fromUtf8("groupBox_Valid"));
        verticalLayout_2 = new QVBoxLayout(groupBox_Valid);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        radioButton_NOT_SEEN_MEANS_UP = new QRadioButton(groupBox_Valid);
        radioButton_NOT_SEEN_MEANS_UP->setObjectName(QString::fromUtf8("radioButton_NOT_SEEN_MEANS_UP"));

        verticalLayout_2->addWidget(radioButton_NOT_SEEN_MEANS_UP);

        radioButton_ALWAYS_VALID = new QRadioButton(groupBox_Valid);
        radioButton_ALWAYS_VALID->setObjectName(QString::fromUtf8("radioButton_ALWAYS_VALID"));

        verticalLayout_2->addWidget(radioButton_ALWAYS_VALID);

        radioButton_VALID_IS_INRANGE = new QRadioButton(groupBox_Valid);
        radioButton_VALID_IS_INRANGE->setObjectName(QString::fromUtf8("radioButton_VALID_IS_INRANGE"));

        verticalLayout_2->addWidget(radioButton_VALID_IS_INRANGE);

        radioButton_VALID_IS_CONFIDENCE = new QRadioButton(groupBox_Valid);
        radioButton_VALID_IS_CONFIDENCE->setObjectName(QString::fromUtf8("radioButton_VALID_IS_CONFIDENCE"));

        verticalLayout_2->addWidget(radioButton_VALID_IS_CONFIDENCE);


        verticalLayout_3->addWidget(groupBox_Valid);

        groupBox_Slot = new QGroupBox(scrollAreaWidgetContents);
        groupBox_Slot->setObjectName(QString::fromUtf8("groupBox_Slot"));
        verticalLayout = new QVBoxLayout(groupBox_Slot);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        radioButton_calculated = new QRadioButton(groupBox_Slot);
        radioButton_calculated->setObjectName(QString::fromUtf8("radioButton_calculated"));
        radioButton_calculated->setChecked(true);

        verticalLayout->addWidget(radioButton_calculated);

        radioButton_SLOT_IS_CONTACTID = new QRadioButton(groupBox_Slot);
        radioButton_SLOT_IS_CONTACTID->setObjectName(QString::fromUtf8("radioButton_SLOT_IS_CONTACTID"));

        verticalLayout->addWidget(radioButton_SLOT_IS_CONTACTID);

        radioButton_SLOT_IS_CONTACTNUMBER = new QRadioButton(groupBox_Slot);
        radioButton_SLOT_IS_CONTACTNUMBER->setObjectName(QString::fromUtf8("radioButton_SLOT_IS_CONTACTNUMBER"));

        verticalLayout->addWidget(radioButton_SLOT_IS_CONTACTNUMBER);

        radioButton_SLOT_IS_CONTACTID_MINUS_ONE = new QRadioButton(groupBox_Slot);
        radioButton_SLOT_IS_CONTACTID_MINUS_ONE->setObjectName(QString::fromUtf8("radioButton_SLOT_IS_CONTACTID_MINUS_ONE"));

        verticalLayout->addWidget(radioButton_SLOT_IS_CONTACTID_MINUS_ONE);

        line = new QFrame(groupBox_Slot);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label = new QLabel(groupBox_Slot);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        radioButton_CYPRESS = new QRadioButton(groupBox_Slot);
        radioButton_CYPRESS->setObjectName(QString::fromUtf8("radioButton_CYPRESS"));

        verticalLayout->addWidget(radioButton_CYPRESS);


        verticalLayout_3->addWidget(groupBox_Slot);

        groupBox_vendor = new QGroupBox(scrollAreaWidgetContents);
        groupBox_vendor->setObjectName(QString::fromUtf8("groupBox_vendor"));
        gridLayout_2 = new QGridLayout(groupBox_vendor);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        checkBox_WIN_8_CERTIFIED = new QCheckBox(groupBox_vendor);
        checkBox_WIN_8_CERTIFIED->setObjectName(QString::fromUtf8("checkBox_WIN_8_CERTIFIED"));

        gridLayout_2->addWidget(checkBox_WIN_8_CERTIFIED, 1, 0, 1, 1);

        checkBox_NO_AREA = new QCheckBox(groupBox_vendor);
        checkBox_NO_AREA->setObjectName(QString::fromUtf8("checkBox_NO_AREA"));

        gridLayout_2->addWidget(checkBox_NO_AREA, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_vendor);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        dockWidget_quirks->setWidget(dockWidgetContents_2);
        QWidget::setTabOrder(scrollArea, radioButton_NOT_SEEN_MEANS_UP);
        QWidget::setTabOrder(radioButton_NOT_SEEN_MEANS_UP, radioButton_ALWAYS_VALID);
        QWidget::setTabOrder(radioButton_ALWAYS_VALID, radioButton_VALID_IS_INRANGE);
        QWidget::setTabOrder(radioButton_VALID_IS_INRANGE, radioButton_VALID_IS_CONFIDENCE);
        QWidget::setTabOrder(radioButton_VALID_IS_CONFIDENCE, radioButton_calculated);
        QWidget::setTabOrder(radioButton_calculated, radioButton_SLOT_IS_CONTACTID);
        QWidget::setTabOrder(radioButton_SLOT_IS_CONTACTID, radioButton_SLOT_IS_CONTACTNUMBER);
        QWidget::setTabOrder(radioButton_SLOT_IS_CONTACTNUMBER, radioButton_SLOT_IS_CONTACTID_MINUS_ONE);

        retranslateUi(Form_Quirks);
        QObject::connect(radioButton_calculated, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_SLOT_IS_CONTACTID, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_SLOT_IS_CONTACTNUMBER, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_SLOT_IS_CONTACTID_MINUS_ONE, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_VALID_IS_CONFIDENCE, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_VALID_IS_INRANGE, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_ALWAYS_VALID, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_NOT_SEEN_MEANS_UP, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(radioButton_CYPRESS, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(checkBox_WIN_8_CERTIFIED, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));
        QObject::connect(checkBox_NO_AREA, SIGNAL(clicked()), Form_Quirks, SLOT(updateQuirks()));

        QMetaObject::connectSlotsByName(Form_Quirks);
    } // setupUi

    void retranslateUi(QWidget *Form_Quirks)
    {
        Form_Quirks->setWindowTitle(QApplication::translate("Form_Quirks", "Form", 0, QApplication::UnicodeUTF8));
        dockWidget_quirks->setWindowTitle(QApplication::translate("Form_Quirks", "Quirks", 0, QApplication::UnicodeUTF8));
        groupBox_Valid->setTitle(QApplication::translate("Form_Quirks", "Valid is...", 0, QApplication::UnicodeUTF8));
        radioButton_NOT_SEEN_MEANS_UP->setText(QApplication::translate("Form_Quirks", "Not Seen means Up", 0, QApplication::UnicodeUTF8));
        radioButton_ALWAYS_VALID->setText(QApplication::translate("Form_Quirks", "Always True", 0, QApplication::UnicodeUTF8));
        radioButton_VALID_IS_INRANGE->setText(QApplication::translate("Form_Quirks", "In Range", 0, QApplication::UnicodeUTF8));
        radioButton_VALID_IS_CONFIDENCE->setText(QApplication::translate("Form_Quirks", "Confidence", 0, QApplication::UnicodeUTF8));
        groupBox_Slot->setTitle(QApplication::translate("Form_Quirks", "Slot is...", 0, QApplication::UnicodeUTF8));
        radioButton_calculated->setText(QApplication::translate("Form_Quirks", "Computed from\n"
"Contact Id", 0, QApplication::UnicodeUTF8));
        radioButton_SLOT_IS_CONTACTID->setText(QApplication::translate("Form_Quirks", "Contact Id", 0, QApplication::UnicodeUTF8));
        radioButton_SLOT_IS_CONTACTNUMBER->setText(QApplication::translate("Form_Quirks", "Contact Number", 0, QApplication::UnicodeUTF8));
        radioButton_SLOT_IS_CONTACTID_MINUS_ONE->setText(QApplication::translate("Form_Quirks", "Contact Id\n"
"Minus One", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form_Quirks", "Vendor specific\n"
"slot functions", 0, QApplication::UnicodeUTF8));
        radioButton_CYPRESS->setText(QApplication::translate("Form_Quirks", "Cypress", 0, QApplication::UnicodeUTF8));
        groupBox_vendor->setTitle(QApplication::translate("Form_Quirks", "General", 0, QApplication::UnicodeUTF8));
        checkBox_WIN_8_CERTIFIED->setText(QApplication::translate("Form_Quirks", "Win 8", 0, QApplication::UnicodeUTF8));
        checkBox_NO_AREA->setText(QApplication::translate("Form_Quirks", "No Area", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form_Quirks: public Ui_Form_Quirks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUIRKS_H
