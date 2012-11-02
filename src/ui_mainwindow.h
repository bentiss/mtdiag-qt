/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "graphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExpert_Mode;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    GraphicsView *graphicsView;
    QSpacerItem *horizontalSpacer;
    QCommandLinkButton *commandLinkButton_disableX11;
    QCommandLinkButton *commandLinkButton_fullscreen;
    QCommandLinkButton *commandLinkButton_fit_to_screen;
    QMenuBar *menubar;
    QMenu *menuAdd_unlisted_device;
    QMenu *menuOrphans_Usb_Hid;
    QStatusBar *statusbar;
    QDockWidget *dockWidgetInputDevices;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QTabWidget *tabWidgetInputDevices;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(917, 521);
        MainWindow->setMinimumSize(QSize(638, 0));
        actionExpert_Mode = new QAction(MainWindow);
        actionExpert_Mode->setObjectName(QString::fromUtf8("actionExpert_Mode"));
        actionExpert_Mode->setCheckable(true);
        actionExpert_Mode->setChecked(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        graphicsView = new GraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setEnabled(false);
        graphicsView->setAcceptDrops(false);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setInteractive(false);

        gridLayout_2->addWidget(graphicsView, 1, 1, 1, 4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 4, 1, 1);

        commandLinkButton_disableX11 = new QCommandLinkButton(centralwidget);
        commandLinkButton_disableX11->setObjectName(QString::fromUtf8("commandLinkButton_disableX11"));
        commandLinkButton_disableX11->setCheckable(true);

        gridLayout_2->addWidget(commandLinkButton_disableX11, 0, 1, 1, 1);

        commandLinkButton_fullscreen = new QCommandLinkButton(centralwidget);
        commandLinkButton_fullscreen->setObjectName(QString::fromUtf8("commandLinkButton_fullscreen"));
        commandLinkButton_fullscreen->setCheckable(true);

        gridLayout_2->addWidget(commandLinkButton_fullscreen, 0, 2, 1, 1);

        commandLinkButton_fit_to_screen = new QCommandLinkButton(centralwidget);
        commandLinkButton_fit_to_screen->setObjectName(QString::fromUtf8("commandLinkButton_fit_to_screen"));
        commandLinkButton_fit_to_screen->setCheckable(true);
        commandLinkButton_fit_to_screen->setChecked(true);

        gridLayout_2->addWidget(commandLinkButton_fit_to_screen, 0, 3, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 917, 25));
        menuAdd_unlisted_device = new QMenu(menubar);
        menuAdd_unlisted_device->setObjectName(QString::fromUtf8("menuAdd_unlisted_device"));
        menuOrphans_Usb_Hid = new QMenu(menuAdd_unlisted_device);
        menuOrphans_Usb_Hid->setObjectName(QString::fromUtf8("menuOrphans_Usb_Hid"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidgetInputDevices = new QDockWidget(MainWindow);
        dockWidgetInputDevices->setObjectName(QString::fromUtf8("dockWidgetInputDevices"));
        dockWidgetInputDevices->setBaseSize(QSize(350, 0));
        dockWidgetInputDevices->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidgetInputDevices = new QTabWidget(dockWidgetContents);
        tabWidgetInputDevices->setObjectName(QString::fromUtf8("tabWidgetInputDevices"));
        tabWidgetInputDevices->setMinimumSize(QSize(300, 0));
        tabWidgetInputDevices->setBaseSize(QSize(350, 0));
        tabWidgetInputDevices->setTabPosition(QTabWidget::West);
        tabWidgetInputDevices->setMovable(true);

        gridLayout->addWidget(tabWidgetInputDevices, 0, 0, 1, 1);

        dockWidgetInputDevices->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetInputDevices);
        QWidget::setTabOrder(commandLinkButton_disableX11, tabWidgetInputDevices);
        QWidget::setTabOrder(tabWidgetInputDevices, graphicsView);

        menubar->addAction(menuAdd_unlisted_device->menuAction());
        menuAdd_unlisted_device->addAction(menuOrphans_Usb_Hid->menuAction());
        menuAdd_unlisted_device->addAction(actionExpert_Mode);

        retranslateUi(MainWindow);
        QObject::connect(commandLinkButton_disableX11, SIGNAL(toggled(bool)), MainWindow, SLOT(disableX11(bool)));
        QObject::connect(commandLinkButton_fullscreen, SIGNAL(clicked(bool)), graphicsView, SLOT(setFullscreen(bool)));
        QObject::connect(commandLinkButton_fit_to_screen, SIGNAL(clicked(bool)), graphicsView, SLOT(setFitToScreen(bool)));
        QObject::connect(commandLinkButton_fullscreen, SIGNAL(clicked(bool)), MainWindow, SLOT(fullscreen(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExpert_Mode->setText(QApplication::translate("MainWindow", "Expert Mode", 0, QApplication::UnicodeUTF8));
        commandLinkButton_disableX11->setText(QApplication::translate("MainWindow", "Auto disable X11 events", 0, QApplication::UnicodeUTF8));
        commandLinkButton_fullscreen->setText(QApplication::translate("MainWindow", "Fullscreen", 0, QApplication::UnicodeUTF8));
        commandLinkButton_fit_to_screen->setText(QApplication::translate("MainWindow", "Fit to screen", 0, QApplication::UnicodeUTF8));
        menuAdd_unlisted_device->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuOrphans_Usb_Hid->setTitle(QApplication::translate("MainWindow", "Orphans Usb Hid", 0, QApplication::UnicodeUTF8));
        dockWidgetInputDevices->setWindowTitle(QApplication::translate("MainWindow", "Devices", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
