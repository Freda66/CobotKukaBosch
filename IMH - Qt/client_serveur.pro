#-------------------------------------------------
#
# Project created by QtCreator 2014-11-28T15:55:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client_serveur
TEMPLATE = app


SOURCES += \
    server.cpp \
    client_serveur.cpp \
    client.cpp \
    main.cpp

HEADERS  += \
    client.h \
    server.h \
    client_serveur.h

FORMS    += \
    client_serveur.ui
