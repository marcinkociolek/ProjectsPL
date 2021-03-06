#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T21:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MKWavelets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../ProjectsLib/LibMarcin/DispLib.cpp
    ../../ProjectsLib/LibMarcin/DispLib.cpp \

HEADERS  += mainwindow.h \
    ../../ProjectsLib/LibMarcin/DispLib.h
    ../../ProjectsLib/LibMarcin/DispLib.h \

FORMS    += mainwindow.ui

win32: INCLUDEPATH += C:\opencv\build\include\
win32: INCLUDEPATH += C:\Boost\include\boost-1_62\
win32: INCLUDEPATH += C:\LibTiff\Include\
win32: INCLUDEPATH += ../../ProjectsLib\LibMarcin\

win32: LIBS += -LC:/opencv/build/x64/vc12/lib
win32: LIBS += -lopencv_core2413d
win32: LIBS += -lopencv_highgui2413d
win32: LIBS += -lopencv_imgproc2413d

win32: LIBS += -LC:\Boost\lib
win32:  LIBS += -lboost_filesystem-vc120-mt-gd-1_62
win32:  LIBS += -lboost_regex-vc120-mt-gd-1_62
