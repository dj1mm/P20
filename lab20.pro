#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T15:33:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab20
TEMPLATE = app


SOURCES += \
    window.cpp \
    sendboard.cpp \
    receiveboard.cpp \
    mywindow.cpp \
    manager.cpp \
    main.cpp \
    abstractcommand.cpp \
    drawline.cpp \
    clearscreen.cpp \
    network.cpp \
    physical.cpp \
    changecolour.cpp

HEADERS  += \
    window.h \
    sendboard.h \
    receiveboard.h \
    mywindow.h \
    manager.h \
    defines.h \
    abstractcommand.h \
    drawline.h \
    clearscreen.h \
    network.h \
    physical.h \
    changecolour.h

FORMS += \
    window.ui

LIBS += -L/usr/local/lib -lwiringPi -lpthread
