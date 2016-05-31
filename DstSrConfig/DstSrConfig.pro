#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T11:49:03
#
#-------------------------------------------------

QT       += widgets xml core

TARGET = DstSrConfig
TEMPLATE = lib

DEFINES += DSTSRCONFIG_LIBRARY

SOURCES += DstSrConfig.cpp

HEADERS += DstSrConfig.h\
        dstsrconfig_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    DstSrConfig.ui
