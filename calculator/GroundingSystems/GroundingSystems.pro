QT += core gui widgets printsupport

CONFIG += c++11

TARGET = GroundingSystems
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    groundingsystem.cpp \
    groundingsystemreport.cpp \
    polynomial.cpp \
    tmath.cpp \
    cable.cpp \
    project.cpp \
    settings.cpp \
    database.cpp \
    qcustomplot.cpp \
    luainterface.cpp \


HEADERS += const.h \
    groundingsystem.h \
    groundingsystemreport.h \
    polynomial.h \
    tmath.h \
    vector3d.h \
    cable.h \
    project.h \
    settings.h \
    database.h \
    qcustomplot.h \
    luainterface.h \


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../../libs
LIBS += -L../../libs \
    -llua5.1 \
    -L/Users/fernandodr/Documents/fluxoEng/lua5.1/ \

INCLUDEPATH += \
    /usr/include/eigen3/ \
    /usr/local/ \
    /Users/fernandodr/Documents/fluxoEng/eigen-eigen-da9b4e14c255/ \
    /Users/fernandodr/Documents/fluxoEng/ \
    $$PWD/src/

