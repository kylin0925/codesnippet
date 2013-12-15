#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T19:33:51
#
#-------------------------------------------------

QT       += core gui
QT +=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = codeSnippet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hightlight.cpp

HEADERS  += mainwindow.h \
    hightlight.h

FORMS    += mainwindow.ui
