TEMPLATE = app

QT += charts testlib
QT += qml quick widgets xml gui core
CONFIG += c++11

SOURCES += main.cpp \
    LoginDlg.cpp \
    MainForm.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(QsLog.pri)

DISTFILES +=

HEADERS += \
    LoginDlg.h \
    MainForm.h

FORMS += \
    LoginDlg.ui \
    MainForm.ui
