######################################################################
# dianvoteplayer.pro
#
# project file for DianVote Player.
#
# Author: tankery.chen@gmail.com
# Modified:
#      date        author      action
#  ------------------------------------------------------------
#      2011-03-15  tankery     created file
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

# basic configuration
PROJECT      = DianVotePlayer
TEMPLATE     = app
TARGET       = DianVotePlayer

CONFIG      += qt warn_on uitools
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
HEADERS     += dianvoteplayer.h \
                slideview.h \
                slidemodel.h
SOURCES     += main.cpp \
				dianvoteplayer.cpp \
                slideview.cpp \
                slidemodel.cpp


# Output paths
DESTDIR      = ./bin
OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
