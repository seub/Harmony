#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T11:10:18
#
#-------------------------------------------------

QT       += core \
    widgets \
    opengl
TARGET = Hitchin
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

LIBS += -L/usr/local/lib -lGLU

TEMPLATE = app

SOURCES += main.cpp \
    discretegroup.cpp \
    topologicalsurface.cpp \
    grouprepresentation.cpp \
    sl2cmatrix.cpp \
    cp1point.cpp \
    h3point.cpp \
    h2point.cpp \
    sl2rmatrix.cpp \
    canvas.cpp \
    h2geodesic.cpp \
    canvasdelegate.cpp \
    h2canvasdelegate.cpp \
    planarline.cpp \
    circle.cpp \
    h2polygon.cpp \
    h2isometry.cpp \
    h3isometry.cpp \
    tools.cpp \
    h3canvasdelegate.cpp\
    fenchelnielsenconstructor.cpp \
    h2buffer.cpp \
    h2mesh.cpp \
    h2triangle.cpp \
    h2trianglesubdivision.cpp \
    h2polygontriangulater.cpp

HEADERS += \
    discretegroup.h \
    topologicalsurface.h \
    grouprepresentation.h \
    sl2cmatrix.h \
    cp1point.h \
    h3point.h \
    h2point.h \
    sl2rmatrix.h \
    canvas.h \
    h2geodesic.h \
    canvasdelegate.h \
    h2canvasdelegate.h \
    planarline.h \
    circle.h \
    h2polygon.h \
    h2isometry.h \
    h3isometry.h \
    tools.h \
    types.h \
    h3canvasdelegate.h\
    fenchelnielsenconstructor.h \
    h2buffer.h \
    h2mesh.h \
    h2triangle.h \
    h2trianglesubdivision.h \
    h2polygontriangulater.h

OTHER_FILES += \
    TODO.txt
