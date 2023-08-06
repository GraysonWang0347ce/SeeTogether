QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    av_queues.cpp \
    decode.cpp \
    demux.cpp \
    main.cpp \
    mainwindow.cpp \
    single_core.cpp

HEADERS += \
    av_queue.hpp \
    av_queues.h \
    decode.h \
    demux.h \
    error.h \
    ffmpeg_header.h \
    mainwindow.h \
    single_core.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


FFMPEG_INCLUDE=../../include/ffmpeg
FFMPEG_LIB=../../lib/ffmpeg
#设置 ffmpeg 的头文件
INCLUDEPATH += $$FFMPEG_INCLUDE

LIBS +=  -L$$FFMPEG_LIB \
         -lavcodec \
         -lavdevice \
         -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswresample \
        -lswscale

RESOURCES += \
    img.qrc
