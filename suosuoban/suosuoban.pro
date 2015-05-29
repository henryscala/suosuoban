#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T20:18:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = suosuoban
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cyclelist.cpp \
    vec2.cpp \
    canvaswidget.cpp \
    geom.cpp

HEADERS  += mainwindow.h \
    cyclelist.h \
    vec2.h \
    canvaswidget.h \
    geom.h

FORMS    += mainwindow.ui \
    canvaswidget.ui
