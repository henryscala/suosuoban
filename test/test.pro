#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T21:52:11
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_testsuosuoban
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ..\\suosuoban

SOURCES += \
        mainwindow.cpp \
    cyclelist.cpp \
    vec2.cpp \
    canvaswidget.cpp \
    geom.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    cyclelist.h \
    vec2.h \
    canvaswidget.h \
    geom.h \
    config.h

SOURCES += tst_testsuosuoban.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
