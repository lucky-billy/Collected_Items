#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T11:08:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FunctionalClientTest
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    qghtcpclient.cpp

HEADERS  += maindialog.h \
    qghtcpclient.h

FORMS    += maindialog.ui
