TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR = build

SOURCES += \
    launcher/main.c

isEmpty(PREFIX) {
 PREFIX = /usr/local
}
target.path = $$PREFIX/bin
setuid.path = $$PREFIX/bin
setuid.extra = chown root '$(INSTALL_ROOT)'$$PREFIX/bin/mtdiag-launch ; \
               chmod +s '$(INSTALL_ROOT)'$$PREFIX/bin/mtdiag-launch
INSTALLS += target setuid

DEFINES += BINDIR='\'"$$PREFIX/bin"\''
