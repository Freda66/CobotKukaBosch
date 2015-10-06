#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T10:53:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebcamCobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui \
    webcam_Frame.ui


INCLUDEPATH += C:\opencv\build\include

LIBS+= -LC:\opencv\build\x64\vc12\lib -lopencv_world300 -lopencv_ts300
