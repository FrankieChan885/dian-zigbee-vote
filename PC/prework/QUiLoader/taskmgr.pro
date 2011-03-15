######################################################################
# taskmgr.pro
#
# project file for Dian Matrix library.
#
# Author: tankery.chen@gmail.com
# Modified:
#      date        author      action
#  ------------------------------------------------------------
#      2011-03-11  tankery     created file
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

# basic configuration
PROJECT      = taskmgr
TEMPLATE     = app

CONFIG      += qt warn_on uitools

# Preprocess definitions
unix {
    DEFINES += _UNIX
} else {
    DEFINES += _WINDOWS
}

# Input
HEADERS     += taskmgr.h
SOURCES     += main.cpp taskmgr.cpp


# Output paths
DESTDIR      = ./bin
OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc

