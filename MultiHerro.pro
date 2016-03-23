#-------------------------------------------------
#
# Project created by QtCreator 2012-07-20T19:49:28
#
#-------------------------------------------------

QT       += core gui

TARGET = MultiHerro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    medium.cpp \
    countMagnetisation.cpp \
    countMagnetisationInPlane.cpp

HEADERS  += mainwindow.h \
    medium.h

FORMS    += mainwindow.ui

INCLUDEPATH +=     /usr/local/qwt/include/

LIBS += -L/usr/local/qwt/lib -lqwt




