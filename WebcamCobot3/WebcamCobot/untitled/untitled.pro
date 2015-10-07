TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


INCLUDEPATH += "C:\Qt\opencv\build\include"

LIBS += -LC:\Qt\opencv\build\x64\vc12\lib -lopencv_world300d
