#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T15:43:43
#
#-------------------------------------------------

QT       += core gui widgets network svg xml

TARGET = CobotKuka2
TEMPLATE = app


SOURCES += main.cpp\
        cobotkuka.cpp \
    json.cpp

HEADERS  += cobotkuka.h \
    json.h

FORMS    += cobotkuka.ui

INCLUDEPATH += "C:\Qt\opencv\build\include"

LIBS += -LC:\Qt\opencv\build\x64\vc12\lib -lopencv_world300d
