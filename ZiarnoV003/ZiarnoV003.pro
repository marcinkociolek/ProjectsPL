#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T11:44:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZiarnoV003
TEMPLATE = app


SOURCES += main.cpp\
        ZiarnoMainWindow.cpp \
    processimage.cpp \
    ../../ProjectsLib/LibMarcin/gradient.cpp \
    ../../ProjectsLib/LibMarcin/DispLib.cpp

HEADERS  += ZiarnoMainWindow.h \
    processimage.h \
    ../../ProjectsLib/LibMarcin/gradient.h \
    ../../ProjectsLib/LibMarcin/DispLib.h \
    ../../ProjectsLibForein/LibPMS/mazdadummy.h \
    ../../ProjectsLibForein/LibPMS/mazdaroi.h \
    ../../ProjectsLibForein/LibPMS/mazdaroiio.h

FORMS    += ZiarnoMainWindow.ui

win32: INCLUDEPATH += C:\opencv\build\include\
#win32: INCLUDEPATH += C:\Boost\include\boost-1_60\boost
win32: INCLUDEPATH += C:\Boost\include\boost-1_62\
win32: INCLUDEPATH += C:\LibTiff\Include\
win32: INCLUDEPATH += ../../ProjectsLib\LibMarcin\
win32: INCLUDEPATH += ../../ProjectsLibForein/LibPMS/

win32: LIBS += -LC:/opencv/build/x64/vc12/lib
win32: LIBS += -lopencv_contrib2413d
win32: LIBS += -lopencv_core2413d
win32: LIBS += -lopencv_highgui2413d
win32: LIBS += -lopencv_imgproc2413d

win32: LIBS += -LC:\Boost\lib
win32:  LIBS += -lboost_filesystem-vc120-mt-gd-1_62
win32:  LIBS += -lboost_regex-vc120-mt-gd-1_62
#win32:  LIBS += -lboost_signals-vc120-mt-gd-1_60


win32: LIBS += -LC:\LibTiff\Bin\libtiff.lib

#QMAKE_CXXFLAGS += -std=c++11
