######################################################################
# dianvoteeditor.pro
#
# project file for DianVote Editor.
#
# Author: tankery.chen@gmail.com
# Modified:
#      date        author      action
#  ------------------------------------------------------------
#      2011-03-15  tankery     created file
#      2011-03-25  tankery     add slideview & slidemodel & resources
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

# basic configuration
PROJECT      = DianVoteEditor
TEMPLATE     = app
TARGET       = DianVoteEditor

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
HEADERS     += dianvoteeditor.h \
                slidescene.h \
                slidemodel.h \
    slideeditor.h
SOURCES     += main.cpp \
				dianvoteeditor.cpp \
                slidescene.cpp \
                slidemodel.cpp \
    slideeditor.cpp


# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
