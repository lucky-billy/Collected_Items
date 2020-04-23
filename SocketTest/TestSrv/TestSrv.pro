#-------------------------------------------------
#
# Project created by QtCreator 2015-01-06T21:38:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestSrv
TEMPLATE = app


SOURCES += main.cpp\
        dialogsrv.cpp \
    clientjobs.cpp

HEADERS  += dialogsrv.h \
    clientjobs.h

FORMS    += dialogsrv.ui
