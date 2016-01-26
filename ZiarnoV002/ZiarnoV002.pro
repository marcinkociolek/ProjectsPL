#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T11:44:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZiarnoV002
TEMPLATE = app


SOURCES += main.cpp\
        ZiarnoMainWindow.cpp \
    processimage.cpp \
    ../../ProjectsLib/LibMarcin/gradient.cpp \
    ../../ProjectsLib/LibMarcin/DispLib.cpp

HEADERS  += ZiarnoMainWindow.h \
    processimage.h \
    ../../ProjectsLib/LibMarcin/gradient.h \
    ../../ProjectsLib/LibMarcin/DispLib.h

FORMS    += ZiarnoMainWindow.ui

win32: INCLUDEPATH += C:\opencv\build\include\
#win32: INCLUDEPATH += C:\Boost\include\boost-1_60\boost
win32: INCLUDEPATH += C:\Boost\include\boost-1_60\
win32: INCLUDEPATH += D:\ProjectsLib\LibMarcin\

win32: LIBS += -LC:/opencv/build/x64/vc12/lib
win32: LIBS += -lopencv_contrib2411d
win32: LIBS += -lopencv_core2411d
win32: LIBS += -lopencv_highgui2411d
win32: LIBS += -lopencv_imgproc2411d

win32: LIBS += -LC:\Boost\lib
win32:  LIBS += -lboost_filesystem-vc120-mt-gd-1_60
win32:  LIBS += -lboost_regex-vc120-mt-gd-1_60
#win32:  LIBS += -lboost_signals-vc120-mt-gd-1_60

#QMAKE_CXXFLAGS += -std=c++11
