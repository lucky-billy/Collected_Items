#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T17:42:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlappyBird
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    qgameelement.cpp \
    qbird.cpp \
    qguide.cpp \
    qground.cpp \
    qpipe.cpp \
    qgameboard.cpp

HEADERS  += mainwindow.h \
    qgameelement.h \
    qbird.h \
    qguide.h \
    qground.h \
    qpipe.h \
    qgameboard.h

RESOURCES += \
    FlappyBird.qrc
