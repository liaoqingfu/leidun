#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T11:36:04
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = senMonitor
TEMPLATE = app
CONFIG += c++11

include(QsLog.pri)

SOURCES += main.cpp \
    UI/MainWindow.cpp \
    UI/LoginDlg.cpp \
    UI/MainForm.cpp \
    UI/DstSrConfig.cpp \
    UI/PlcSrConfig.cpp \
    UI/MonitorDlg.cpp \
    Uart/SerialPortList.cpp \
    Uart/qextserialport.cpp


HEADERS  += UI/MainWindow.h \
    UI/LoginDlg.h \
    UI/MainForm.h \
    UI/DstSrConfig.h \
    UI/PlcSrConfig.h \
    UI/MonitorDlg.h \
    Uart/SerialPortList.h \
    Uart/qextserialport.h \
    Uart/qextserialport_global.h \
    Uart/qextserialport_p.h

win32 {
    SOURCES += Uart/qextserialport_win.cpp
}
unix {
     SOURCES += Uart/qextserialport_unix.cpp
}

FORMS    += UI/MainWindow.ui \
    UI/LoginDlg.ui \
    UI/MainForm.ui \
    UI/DstSrConfig.ui \
    UI/PlcSrConfig.ui \
    UI/MonitorDlg.ui

DISTFILES += \
    qextserial/LICENSE

