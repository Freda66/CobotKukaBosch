#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T10:53:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = WebcamCobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += "C:\Qt\opencv\build\include"

LIBS += -LC:\Qt\opencv\build\x64\vc12\lib -lopencv_world300d
