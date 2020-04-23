#-------------------------------------------------
#
# Project created by QtCreator 2014-02-15T21:11:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiTouch
TEMPLATE = app


SOURCES += main.cpp\
        multitouch.cpp

HEADERS  += multitouch.h

FORMS    += multitouch.ui

CONFIG += mobility
MOBILITY = 

QMAKE_CXXFLAGS += -std=c++11
