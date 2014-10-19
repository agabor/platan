#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T14:33:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StatementAnonymizator
TEMPLATE = app

CONFIG  += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    validation.cpp \
    substitutetablemodel.cpp

HEADERS  += mainwindow.h \
    validation.h \
    substitutetablemodel.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../Platan/PlatanCSV

include(../Platan/PlatanCSV/PlatanCSV.pri)

RESOURCES += \
    icons.qrc

RC_FILE = myapp.rc
