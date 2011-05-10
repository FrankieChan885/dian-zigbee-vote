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
    qtwin.cpp \
    hidcontrol.cpp

HEADERS  += dianvotecontrol.h \
    drawhistgram.h \
    drawpie.h \
    dianvotedrawer.h \
    drawbase.h \
    hidlistener.h \
    hiddevice.h \
    qtwin.h \
    hidcontrol.h

# using hidapi
LIBS += -L../bin -lhidapi

FORMS    += dianvotecontrol.ui \
    dianvotedrawer.ui

RESOURCES += \
    DianVoteDrawer.qrc

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
RCC_DIR      = ./temp
UI_DIR       = ./temp
