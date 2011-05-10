#-------------------------------------------------
#
# Project created by QtCreator 2011-04-23T21:55:34
#
#-------------------------------------------------

QT       += core gui

TARGET = DianVoteDrawer
TEMPLATE = app

SOURCES += main.cpp\
        dianvotedrawer.cpp \
    histgram.cpp \
    pie.cpp

HEADERS  += dianvotedrawer.h \
    histgram.h \
    datatype.h \
    pie.h

FORMS    += dianvotedrawer.ui

#RESOURCES   += DianVoteDrawer.qrc

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
RCC_DIR      = ./temp
UI_DIR      = ./temp

RESOURCES += \
    DianVoteDrawer.qrc
