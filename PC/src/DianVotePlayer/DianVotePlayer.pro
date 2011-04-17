######################################################################
# dianvoteplayer.pro
#
# project file for DianVote Player.
#
# Author: tankery.chen@gmail.com
# Date: 2011-04-03
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

# basic configuration
PROJECT      = DianVotePlayer
TEMPLATE     = app
TARGET       = DianVotePlayer

CONFIG      += qt warn_on
QT          += xml

DEPENDPATH  += ../utilities
INCLUDEPATH += ../utilities

# Preprocess definitions
unix {
    DEFINES += _UNIX
} else {
    DEFINES += _WINDOWS
}

# Input
HEADERS     += slidescene.h \
                slideview.h \
                slidemodel.h \
                slidetextitem.h \
                hiddevice.h \
                hidlistener.h

SOURCES     += main.cpp \
                slidescene.cpp \
                slideview.cpp \
                slidemodel.cpp \
                slidetextitem.cpp \
                hiddevice.cpp \
                hidlistener.cpp

LIBS        += -lhid

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp

