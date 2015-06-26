#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T20:18:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = suosuoban
TEMPLATE = app

#added by me
QMAKE_CXXFLAGS_RELEASE += -g
QMAKE_CFLAGS_RELEASE += -g
QMAKE_LFLAGS_RELEASE = -mthreads
QMAKE_CXXFLAGS_DEBUG += -g
QMAKE_CFLAGS_DEBUG += -g
QMAKE_LFLAGS_DEBUG = -mthreads


SOURCES += main.cpp\
        mainwindow.cpp \
    geom.cpp \
    config.cpp \
    canvasscene.cpp \
    qmypathitem.cpp \
    grid.cpp \
    myalgorithm.cpp \
    ccrashstack.cpp \
    persistence.cpp \
    history.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    geom.h \
    config.h \
    canvasscene.h \
    qmypathitem.h \
    grid.h \
    myalgorithm.h \
    ccrashstack.h \
    persistence.h \
    history.h \
    configdialog.h

FORMS    += mainwindow.ui \
    configdialog.ui
