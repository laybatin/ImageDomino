#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T23:09:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Domino
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myqgraphicsview.cpp \
    settingdialog.cpp

HEADERS  += mainwindow.h \
    myqgraphicsview.h \
    settingdialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui

RESOURCES += \
    ActionBarIcon.qrc

CONFIG += static
