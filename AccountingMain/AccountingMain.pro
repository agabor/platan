TEMPLATE = app

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += ordered c++11 qscintilla2

QMAKE_CXXFLAGS += -O0 -g -Werror=return-type
QMAKE_CXXFLAGS += $$system(python3.2-config --cflags)
QMAKE_CXXFLAGS -= -Wstrict-prototype
QMAKE_CXXFLAGS -= -O2
LIBS += $$system(python3.2-config --ldflags)

INCLUDEPATH += SQLiteWrapper \
               AccountDataBase \
               AccountingUI \
               AccountingApp \
               PythonAPI \
               Statements \
               /usr/include/python3.2

include(SQLiteWrapper/SQLiteWrapper.pri)
include(AccountDataBase/AccountDataBase.pri)
include(AccountingUI/AccountingUI.pri)
include(AccountingApp/AccountingApp.pri)
include(Statements/Statements.pri)
include(PythonAPI/PythonAPI.pri)
