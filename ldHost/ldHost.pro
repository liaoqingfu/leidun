TEMPLATE = app

QT += charts testlib
QT += qml quick widgets xml gui core
CONFIG += c++11

SOURCES += main.cpp \
    UI/LoginDlg.cpp \
    UI/MainForm.cpp \
    UI/RootWindow.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(QsLog.pri)

DISTFILES +=

HEADERS += \
    UI/LoginDlg.h \
    UI/MainForm.h \
    UI/RootWindow.h

FORMS += \
    UI/LoginDlg.ui \
    UI/MainForm.ui \
    UI/RootWindow.ui
