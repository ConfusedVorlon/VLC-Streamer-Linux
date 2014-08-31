#-------------------------------------------------
#
# Copyright (C) 2011 Jens Stockhausen
# Contact: (vlc@famst.de)
#
# This file is part of the VLCStreamerClient application.
#
#-------------------------------------------------

QT += core gui network

TEMPLATE = app

VERSION = 0.1.3_beta

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG += debug_and_release

CONFIG(debug, debug|release) {
    TARGET  = VLCStreamerClientD
    DESTDIR = ./../binD
} else {
    TARGET  = VLCStreamerClient
    DESTDIR = ./../bin
}


SOURCES += main.cpp\
        MainDialog.cpp \
    Zeroconf.cpp \
    HttpServer.cpp \
    VLCStreamer.cpp \
    UrlParser.cpp \
    MoviesHandler.cpp \
    Logger.cpp \
    VLCEncoder.cpp \
    CreateHandler.cpp

HEADERS  += MainDialog.h \
    Zeroconf.h \
    HttpServer.h \
    VLCStreamer.h \
    UrlParser.h \
    MoviesHandler.h \
    Logger.h \
    VLCEncoder.h \
    CreateHandler.h

FORMS    += MainDialog.ui

RESOURCES += \
    resources.qrc
