#-------------------------------------------------
#
# Project created by QtCreator 2011-11-18T09:53:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mtdiag-qt
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += libevdev

LIBS += -ludev -lX11 -lXi
INCLUDEPATH += /media/data/Src/Kernel/all/include/

#CONFIG(debug, debug|release) {
#    DESTDIR = build/debug
#} else {
#    DESTDIR = build/release
#}

DESTDIR = build

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

SOURCES += \
    src/xi2manager.cpp \
    src/xi2device.cpp \
    src/vidpidaction.cpp \
    src/udevdevice.cpp \
    src/udev.cpp \
    src/touch.cpp \
    src/qdevice.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/kernelDevice.cpp \
    src/hidmt.cpp \
    src/graphicsview.cpp \
    src/drawingtouch.cpp \
    src/colorwidget.cpp \
    src/deviceview.cpp \
    launcher/launcher_mtdiag_launcher.c

HEADERS  += \
    src/xi2manager.h \
    src/xi2device.h \
    src/vidpidaction.h \
    $${DESTDIR}/.ui/ui_quirks.h \
    $${DESTDIR}/.ui/ui_protocol.h \
    $${DESTDIR}/.ui/ui_mainwindow.h \
    $${DESTDIR}/.ui/ui_input.h \
    src/udevdevice.h \
    src/udev.h \
    src/touch.h \
    src/qdevice.h \
    src/mainwindow.h \
    src/kernelDevice.h \
    src/hidmt.h \
    src/hid.h \
    src/graphicsview.h \
    src/drawingtouch.h \
    src/colorwidget.h \
    src/x11.h \
    src/deviceview.h \
    launcher/launcher_mtdiag_launcher.h

FORMS    += \
    forms/quirks.ui \
    forms/protocol.ui \
    forms/mainwindow.ui \
    forms/input.ui

OTHER_FILES +=

RESOURCES += \
    resources/icons.qrc

isEmpty(PREFIX) {
 PREFIX = /usr/local
}
target.path = $$PREFIX/bin
INSTALLS += target
























