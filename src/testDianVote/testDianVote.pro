######################################################################
# Automatically generated by qmake (2.01a) Fri Apr 15 15:02:09 2011
######################################################################

TEMPLATE = app
TARGET = 

DEPENDPATH  += ../utilities
INCLUDEPATH += ../utilities \
                ../hidapi/hidapi-0.6.0/hidapi

# using hidapi
LIBS += -L../bin -lhidapi

# not using libhid
# LIBS += -lhid
# DEFINES += USE_LIBHID

# Input
HEADERS += \
           hiddevice.h \
           hidlistener.h \
           testdianvote.h \
           hidcontrol.h
SOURCES += \
           hiddevice.cpp \
           hidlistener.cpp \
           main.cpp \
           testdianvote.cpp \
           hidcontrol.cpp

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
RCC_DIR      = ./temp
UI_DIR       = ./temp

FORMS += \
    testdianvote.ui