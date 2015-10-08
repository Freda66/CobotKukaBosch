#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T15:43:43
#
#-------------------------------------------------

QT       += core gui widgets network svg xml

TARGET = CobotKuka
TEMPLATE = app


SOURCES += main.cpp\
        cobotkuka.cpp \
    drawwidget.cpp \
    mainwindow.cpp

HEADERS  += cobotkuka.h \
    drawwidget.h \
    mainwindow.h

FORMS    += cobotkuka.ui \
    mainwindow.ui

INCLUDEPATH += "C:\opencv\build\include"

LIBS += -LC:\opencv\build\x64\vc12\lib -lopencv_world300
