#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T11:10:18
#
#-------------------------------------------------

QT       += core \
    widgets

QT       -= gui

TARGET = Hitchin
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    type.cpp \
    discretegroup.cpp \
    topologicalsurface.cpp \
    grouprepresentation.cpp \
    sl2cmatrix.cpp \
    cp1point.cpp \
    h3point.cpp \
    h2point.cpp \
    sl2rmatrix.cpp \
    h2canvas.cpp \
    canvas.cpp \
    h2geodesic.cpp \
    linesinc.cpp \
    canvas.cpp \
    canvasdelegate.cpp \
    h2canvasdelegate.cpp

HEADERS += \
    types.h \
    discretegroup.h \
    topologicalsurface.h \
    grouprepresentation.h \
    sl2cmatrix.h \
    cp1point.h \
    h3point.h \
    h2point.h \
    sl2rmatrix.h \
    h2canvas.h \
    canvas.h \
    h2geodesic.h \
    linesinc.h
    canvas.h \
    canvasdelegate.h \
    h2canvasdelegate.h
