#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T15:04:42
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = premierTest
TEMPLATE = app

macx {
    QMAKE_MAC_SDK = macosx10.9
}
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
