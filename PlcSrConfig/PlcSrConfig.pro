#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T11:59:43
#
#-------------------------------------------------

QT       += core widgets xml

TARGET = PlcSrConfig
TEMPLATE = lib

DEFINES += PLCSRCONFIG_LIBRARY

SOURCES += PlcSrConfig.cpp

HEADERS += PlcSrConfig.h\
        plcsrconfig_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    PlcSrConfig.ui
