#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T22:03:23
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projeto
TEMPLATE = app

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    light.cpp \
    material.cpp \
    camera.cpp \
    trackball.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    light.h \
    material.h \
    camera.h \
    trackball.h \
    model.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc \
    resources.qrc
