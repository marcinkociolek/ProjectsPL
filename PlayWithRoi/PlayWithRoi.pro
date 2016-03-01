#-------------------------------------------------
#
# Project created by QtCreator 2016-03-01T12:23:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlayWithRoi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        ../../ProjectsLib/LibMarcin/DispLib.cpp\
        ../../ProjectsLib/LibMarcin/RegionU16Lib.cpp

HEADERS  += mainwindow.h\
         ../../ProjectsLib/LibMarcin/DispLib.h\
        ../../ProjectsLib/LibMarcin/RegionU16Lib.h

FORMS    += mainwindow.ui

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
