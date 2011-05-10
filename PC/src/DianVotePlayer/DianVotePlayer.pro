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
DEPENDPATH  += ../hidapi/lib
INCLUDEPATH += ../utilities
INCLUDEPATH += ../hidapi/hidapi-0.6.0/hidapi

# Preprocess definitions
unix {
    DEFINES += _UNIX
} else {
    DEFINES += _WINDOWS
}

# Input
HEADERS     += slidescene.h \
                slideplayerscene.h \
                slidemodel.h \
                slidetextitem.h \
                hiddevice.h \
                hidlistener.h

SOURCES     += main.cpp \
                slidescene.cpp \
                slideplayerscene.cpp \
                slidemodel.cpp \
                slidetextitem.cpp \
                hiddevice.cpp \
                hidlistener.cpp

LIBS        += -L../bin -lhidapi

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp

