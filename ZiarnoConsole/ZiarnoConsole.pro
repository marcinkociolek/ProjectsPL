QT += core
QT -= gui

CONFIG += c++11

TARGET = ZiarnoConsole
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp\
    SegmentGrainImage.cpp \
    #../../ProjectsLib/LibMarcin/DispLib.cpp \
    gradient.cpp


HEADERS  += SegmentGrainImage.h \
    gradient.h \
    #../../ProjectsLib/LibMarcin/DispLib.h \
    ../../ProjectsLibForein/LibPMS/mazdadummy.h \
    ../../ProjectsLibForein/LibPMS/mazdaroi.h \
    ../../ProjectsLibForein/LibPMS/mazdaroiio.h


win32: INCLUDEPATH += C:\opencv\build\include\
#win32: INCLUDEPATH += C:\Boost\include\boost-1_62\
win32: INCLUDEPATH += C:\LibTiff\Include\
win32: INCLUDEPATH += ../../ProjectsLib\LibMarcin\
win32: INCLUDEPATH += ../../ProjectsLibForein/LibPMS/

win32: LIBS += -LC:/opencv/build/x64/vc12/lib
win32: LIBS += -lopencv_core2413d
win32: LIBS += -lopencv_highgui2413d
win32: LIBS += -lopencv_imgproc2413d

#win32: LIBS += -LC:\Boost\lib
#win32:  LIBS += -lboost_filesystem-vc120-mt-gd-1_62
#win32:  LIBS += -lboost_regex-vc120-mt-gd-1_62


win32: LIBS += -LC:\LibTiff\Bin
win32: LIBS += -llibtiff_i
