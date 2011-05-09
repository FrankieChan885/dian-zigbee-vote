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
INCLUDEPATH += ../include
INCLUDEPATH += ../hidapi/hidapi-0.6.0/hidapi

SOURCES += main.cpp\
        dianvotecontrol.cpp \
    drawhistgram.cpp \
    drawpie.cpp \
    dianvotedrawer.cpp \
    drawbase.cpp \
    hidlistener.cpp \
    hiddevice.cpp \
    qtwin.cpp

HEADERS  += dianvotecontrol.h \
    drawhistgram.h \
    drawpie.h \
    dianvotedrawer.h \
    drawbase.h \
    hidlistener.h \
    hiddevice.h \
    qtwin.h

# using hidapi
LIBS += -L../hidapi/lib -lhidapi

FORMS    += dianvotecontrol.ui \
    dianvotedrawer.ui

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
RES_DIR      = ./temp

RESOURCES += \
    DianVoteDrawer.qrc
