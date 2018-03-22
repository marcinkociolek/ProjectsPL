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
    ../../ProjectsLib/LibMarcin/DispLib.cpp \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/gradient.cpp \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/heightfromsideimage.cpp \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/SegmentGrainImage.cpp

HEADERS  += ZiarnoMainWindow.h \
    processimage.h \
    ../../ProjectsLib/LibMarcin/DispLib.h \
    ../../ProjectsLibForein/LibPMS/mazdadummy.h \
    ../../ProjectsLibForein/LibPMS/mazdaroi.h \
    ../../ProjectsLibForein/LibPMS/mazdaroiio.h \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/gradient.h \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/heightfromsideimage.h \
    ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/SegmentGrainImage.h

FORMS    += ZiarnoMainWindow.ui

win32: INCLUDEPATH += C:\opencv\build\include\
#win32: INCLUDEPATH += C:\Boost\include\boost-1_60\boost
win32: INCLUDEPATH += C:\boost_1_66_0\
win32: INCLUDEPATH += C:\LibTiff\
win32: INCLUDEPATH += ../../ProjectsLib\LibMarcin\
win32: INCLUDEPATH += ../../ProjectsLibForein/LibPMS/
win32: INCLUDEPATH += ../../ProjectsLibForein/LibPMS/

win32: INCLUDEPATH += ../../ProjectsGitLab/ZiarnoModuleTemp/Segmentacja/

# this is for debug
#win32: LIBS += -LC:/opencv/build/x64/vc15/lib/
#win32: LIBS += -lopencv_world341d

#win32: LIBS += -LC:/boost_1_66_0/stage/x64/lib/
#win32:  LIBS += -lboost_filesystem-vc141-mt-gd-x64-1_66
#win32:  LIBS += -lboost_regex-vc141-mt-gd-x64-1_66


# this is for release
win32: LIBS += -LC:/opencv/build/x64/vc15/lib/
win32: LIBS += -lopencv_world341

win32: LIBS += -LC:/boost_1_66_0/stage/x64/lib/
win32:  LIBS += -lboost_filesystem-vc141-mt-x64-1_66
win32:  LIBS += -lboost_regex-vc141-mt-x64-1_66

win32: LIBS += -LC:/LibTiff/
win32: LIBS += -llibtiff_i

#QMAKE_CXXFLAGS += -std=c++11
