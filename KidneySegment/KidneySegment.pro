#-------------------------------------------------
#
# Project created by QtCreator 2017-09-01T11:05:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KidneySegment
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        ../../ProjectsLib/LibMarcin/DispLib.cpp \
    ../../ProjectsLib/LibMarcin/StringFcLib.cpp \
    ../../ProjectsLib/LibMarcin/gradient.cpp \
    myimagewidget.cpp \
    ../../ProjectsLib/LibMarcin/RegionU16Lib.cpp

HEADERS  += mainwindow.h\
        ../../ProjectsLib/LibMarcin/DispLib.h \
        ../../ProjectsLibForein/LibPMS/mazdadummy.h \
        ../../ProjectsLibForein/LibPMS/mazdaroi.h \
        ../../ProjectsLibForein/LibPMS/mazdaroiio.h \
        ../../ProjectsLib/LibMarcin/StringFcLib.h \
    ../../ProjectsLib/LibMarcin/gradient.h \
    myimagewidget.h \
    ../../ProjectsLib/LibMarcin/RegionU16Lib.h

FORMS    += mainwindow.ui

win32: INCLUDEPATH += C:\opencv\build\include\
win32: INCLUDEPATH += C:\boost_1_66_0\
win32: INCLUDEPATH += C:\LibTiff\
win32: INCLUDEPATH += ../../ProjectsLib\LibMarcin\
win32: INCLUDEPATH += ../../ProjectsLibForein/LibPMS/

# this is for debug
win32: LIBS += -LC:/opencv/build/x64/vc15/lib/
win32: LIBS += -lopencv_world341d

win32: LIBS += -LC:/boost_1_66_0/stage/x64/lib/
win32:  LIBS += -lboost_filesystem-vc141-mt-gd-x64-1_66
win32:  LIBS += -lboost_regex-vc141-mt-gd-x64-1_66


# this is for release
#win32: LIBS += -LC:/opencv/build/x64/vc15/lib/
#win32: LIBS += -lopencv_world341

#win32: LIBS += -LC:/boost_1_66_0/stage/x64/lib/
#win32:  LIBS += -lboost_filesystem-vc141-mt-x64-1_66
#win32:  LIBS += -lboost_regex-vc141-mt-x64-1_66


win32: LIBS += -LC:/LibTiff/
win32: LIBS += -llibtiff_i

