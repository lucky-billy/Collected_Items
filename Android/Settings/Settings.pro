#-------------------------------------------------
#
# Project created by QtCreator 2015-02-13T21:48:50
#
#-------------------------------------------------

QT       += core gui

QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Settings
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ../../qt/qDebug2Logcat.cpp

HEADERS  += widget.h \
    ../../qt/qDebug2Logcat.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

