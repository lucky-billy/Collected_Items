#-------------------------------------------------
#
# Project created by QtCreator 2010-07-14T17:54:25
#
#-------------------------------------------------
#if QT_VERSION >= 0x050000
QT       += core gui widgets
#else
QT       += core gui
#endif

TARGET = qcom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
        qextserial/qextserialport.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
        qextserial/qextserialport_global.h \
        qextserial/qextserialport.h

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    images.qrc
RC_FILE += myico.rc

OTHER_FILES += \
    myico.rc
