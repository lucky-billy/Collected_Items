#-------------------------------------------------
#
# Project created by QtCreator 2014-04-30T10:42:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RTSPTool
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    qffmpeg.cpp \
    rtspthread.cpp

HEADERS  += frmmain.h \
    qffmpeg.h \
    rtspthread.h

FORMS    += frmmain.ui

INCLUDEPATH += $$PWD/ffmpeg/include
LIBS += $$PWD/ffmpeg/lib/libavcodec.dll.a \
        $$PWD/ffmpeg/lib/libavfilter.dll.a \
        $$PWD/ffmpeg/lib/libavformat.dll.a \
        $$PWD/ffmpeg/lib/libswscale.dll.a \
        $$PWD/ffmpeg/lib/libavutil.dll.a \

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin

win32:RC_FILE=main.rc

RESOURCES += \
    res.qrc
