#-------------------------------------------------
#
# Project created by QtCreator 2011-04-19T10:33:11
#
#-------------------------------------------------

QWT_ROOT = $${PWD}/../qwt
include( $${QWT_ROOT}/qwtconfig.pri )
include( $${QWT_ROOT}/qwtbuild.pri )

TEMPLATE     = app

INCLUDEPATH += $${QWT_ROOT}/src
DEPENDPATH  += $${QWT_ROOT}/src

QMAKE_RPATHDIR *= $${QWT_ROOT}/lib

contains(QWT_CONFIG, QwtFramework) {

    LIBS      += -F$${QWT_ROOT}/lib
}
else {

    LIBS      += -L$${QWT_ROOT}/lib
}

IPATH       = $${INCLUDEPATH}
qtAddLibrary(qwt)
INCLUDEPATH = $${IPATH}

contains(QWT_CONFIG, QwtSvg) {

    QT += svg
}
else {

    DEFINES += QWT_NO_SVG
}


win32 {
    contains(QWT_CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
    }
}

QT       += core gui

TARGET = DianVoteDrawer
TEMPLATE = app

CONFIG      += qt warn_on uitools
QT          += xml

# Preprocess definitions
unix {
    DEFINES += _UNIX
} else {
    DEFINES += _WINDOWS
}

SOURCES += main.cpp\
        dianvotedrawer.cpp \
    histgram.cpp \
    pie.cpp

HEADERS  += dianvotedrawer.h \
    histgram.h \
    pie.h

FORMS    += dianvotedrawer.ui

# Output paths
DESTDIR      = ../bin
OBJECTS_DIR  = ./temp
MOC_DIR      = ./temp
