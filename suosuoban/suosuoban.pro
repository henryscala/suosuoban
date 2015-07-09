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
TRANSLATIONS = languages/lang_en.ts  languages/lang_zh.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    geom.cpp \
    config.cpp \
    canvasscene.cpp \
    qmypathitem.cpp \
    grid.cpp \
    myalgorithm.cpp \
    persistence.cpp \
    history.cpp \
    configdialog.cpp \
    mylocale.cpp

HEADERS  += mainwindow.h \
    geom.h \
    config.h \
    canvasscene.h \
    qmypathitem.h \
    grid.h \
    myalgorithm.h \
    persistence.h \
    history.h \
    configdialog.h \
    mylocale.h

FORMS    += mainwindow.ui \
    configdialog.ui
