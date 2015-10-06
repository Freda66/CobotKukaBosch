#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T10:38:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draw
TEMPLATE = app


macx {
    QMAKE_MAC_SDK = macosx10.9
}

SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp

HEADERS  += mainwindow.h \
    drawwidget.h

FORMS    += mainwindow.ui
