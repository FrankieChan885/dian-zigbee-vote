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

# basic configuration
PROJECT      = hidapi
TEMPLATE     = lib
TARGET   = hidapi

CONFIG      += warn_on dll
CONFIG      -= qt

DEPENDPATH  += hidapi-0.6.0/hidapi
win32:DEPENDPATH += hidapi-0.6.0/windows
unix:DEPENDPATH += hidapi-0.6.0/linux

INCLUDEPATH += hidapi-0.6.0/hidapi

unix:QMAKE_CC += `pkg-config libusb-1.0 --cflags`
unix:QMAKE_CXX += `pkg-config libusb-1.0 --cflags`
unix:QMAKE_LIBS += `pkg-config libusb-1.0 libudev --libs`
win32:LIBS    += -lsetupapi

# Input
HEADERS         += hidapi.h
unix:SOURCES    += hid-libusb.c
win32:SOURCES   += hid.cpp

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp

