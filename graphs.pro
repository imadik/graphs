#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T22:27:14
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphs
TEMPLATE = app


SOURCES += main.cpp\
        mainwin.cpp \
    widget.cpp \
    functhread.cpp \
    opensavewindow.cpp

HEADERS  += mainwin.h \
    widget.h \
    functhread.h \
    opensavewindow.h

FORMS    += mainwin.ui \
    opensavewindow.ui
