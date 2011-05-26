#-------------------------------------------------
#
# Project created by QtCreator 2011-05-03T15:17:56
#
#-------------------------------------------------

QT       += core gui

TARGET = DianVoteControl
TEMPLATE = app

DEPENDPATH  += ../utilities
DEPENDPATH  += ../lib
INCLUDEPATH += ../utilities
INCLUDEPATH += ../hidapi/hidapi-0.6.0/hidapi

SOURCES += main.cpp\
        dianvotecontrol.cpp \
    drawhistgram.cpp \
    drawpie.cpp \
    dianvotedrawer.cpp \
    drawbase.cpp \
    hidlistener.cpp \
    hiddevice.cpp \
    hidcontrol.cpp \
    stopwatch.cpp \
    changestatus.cpp \
    dianvoteoption.cpp

HEADERS  += dianvotecontrol.h \
    drawhistgram.h \
    drawpie.h \
    dianvotedrawer.h \
    drawbase.h \
    hidlistener.h \
    hiddevice.h \
    hidcontrol.h \
    stopwatch.h \
    dianvotedebug.h \
    dianvoteoption.h

win32:SOURCES += qtwin.cpp
win32:HEADERS += qtwin.h

# using hidapi
LIBS += -L../bin -lhidapi

FORMS    += dianvotecontrol.ui \
    dianvotedrawer.ui \
    dianvoteoptions.ui


# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
RCC_DIR      = ./temp
UI_DIR       = ./temp
