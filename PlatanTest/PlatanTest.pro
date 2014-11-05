#-------------------------------------------------
#
# Project created by QtCreator 2014-02-23T13:22:02
#
#-------------------------------------------------

QT       += core testlib sql gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_sqlite
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -Werror=return-type -g -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0

LIBS += -lgcov

SOURCES += \
    tst_sqlite.cpp \
    main.cpp \
    csvtest.cpp \
    qxlsdocumenttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../Platan/SQLiteWrapper \
               ../Platan/PlatanData \
               ../Platan/PlatanCSV \
               ../Platan/QLibXLS

include(../Platan/SQLiteWrapper/SQLiteWrapper.pri)
include(../Platan/PlatanData/PlatanData.pri)
include(../Platan/PlatanCSV/PlatanCSV.pri)
include(../Platan/QLibXLS/QLibXLS.pri)

HEADERS += \
    tst_sqlite.h \
    csvtest.h \
    qxlsdocumenttest.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lxlsreader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -lxlsreader
else:unix: LIBS += -L$$PWD/../lib/ -lxlsreader

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

OTHER_FILES += \
    ../sample/numbers.xls
