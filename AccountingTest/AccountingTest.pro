#-------------------------------------------------
#
# Project created by QtCreator 2014-02-23T13:22:02
#
#-------------------------------------------------

QT       += core testlib sql

TARGET = tst_sqlite
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Werror=return-type

SOURCES += \
    tst_sqlite.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../AccountingMain/SQLiteWrapper \
               ../AccountingMain/AccountDataBase

include(../AccountingMain/SQLiteWrapper/SQLiteWrapper.pri)
include(../AccountingMain/AccountDataBase/AccountDataBase.pri)

HEADERS +=
