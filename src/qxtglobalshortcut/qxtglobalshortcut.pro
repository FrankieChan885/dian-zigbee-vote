######################################################################
# hidapi.pro
#
# project file for HIDAPI.
#
# Author: tankery.chen@gmail.com
# Data: 2011-05-01
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

QT       += core gui

# include pri
include(qxtglobalshortcut.pri)

# basic configuration
PROJECT		= qxtglobalshortcut
TEMPLATE    = lib
TARGET      = qxtglobalshortcut

CONFIG      += warn_on dll

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
