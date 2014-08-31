#-------------------------------------------------
#
# Copyright (C) 2011 Jens Stockhausen
# Contact: (vlc@famst.de)
#
# This file is part of the VLCStreamerClient application.
#
#-------------------------------------------------


QT       += network testlib
CONFIG   += console
TEMPLATE = app


CONFIG(debug, debug|release) {
    TARGET  = tst_DispatcherTestD
    DESTDIR = ./../binD
} else {
    TARGET  = tst_DispatcherTest
    DESTDIR = ./../bin
}

INCLUDEPATH += ../VLCStreamerClient/

SOURCES += \
    main.cpp \
    UrlParserTest.cpp \
    MoviesTest.cpp \
    ../VLCStreamerClient/UrlParser.cpp \
    ../VLCStreamerClient/MoviesHandler.cpp \
    ../VLCStreamerClient/Logger.cpp


HEADERS += \
    UrlParserTest.h \
    MoviesTest.h \
    ../VLCStreamerClient/UrlParser.h \
    ../VLCStreamerClient/MoviesHandler.h \
    ../VLCStreamerClient/Logger.h
