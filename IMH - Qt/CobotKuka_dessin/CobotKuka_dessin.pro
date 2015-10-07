#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T15:43:43
#
#-------------------------------------------------

QT       += core gui widgets network svg xml

TARGET = CobotKuka_dessin
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += main.cpp\
        cobotkuka.cpp \
    mainwindow.cpp \
    drawwidget.cpp

HEADERS  += cobotkuka.h \
    mainwindow.h \
    drawwidget.h

FORMS    += cobotkuka.ui \
    mainwindow.ui


