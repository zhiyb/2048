#-------------------------------------------------
#
# Project created by QtCreator 2014-05-06T12:46:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2048
TEMPLATE = app
CONFIG += static

SOURCES += main.cpp \
    grid.cpp \
    tile.cpp \
    cell.cpp \
    animation.cpp \
    mainw.cpp

HEADERS  += grid.h \
    view.h \
    tile.h \
    cell.h \
    animation.h \
    mainw.h

RESOURCES += \
    image.qrc
