#-------------------------------------------------
#
# Project created by QtCreator 2011-11-18T09:53:20
#
#-------------------------------------------------

QT       += core gui

TARGET = mtdiag-qt
TEMPLATE = app

LIBS += -ludev -lX11 -lXi
INCLUDEPATH += /media/data/Src/Kernel/all/include/

SOURCES += main.cpp\
        mainwindow.cpp \
    udev.cpp \
    qdevice.cpp \
    touch.cpp \
    colorwidget.cpp \
    kernelDevice.cpp \
    udevdevice.cpp \
    xi2manager.cpp \
    xi2device.cpp \
    hidmt.cpp \
    graphicsview.cpp \
    vidpidaction.cpp

HEADERS  += mainwindow.h \
    udev.h \
    qdevice.h \
    touch.h \
    colorwidget.h \
    kernelDevice.h \
    udevdevice.h \
    xi2manager.h \
    xi2device.h \
    hidmt.h \
    graphicsview.h \
    vidpidaction.h \
    hid.h

FORMS    += mainwindow.ui \
    quirks.ui \
    input.ui \
    protocol.ui

OTHER_FILES +=

RESOURCES += \
    icons.qrc

























