
SOURCES += \
    ../AccountingMain/AccountDataBase/csvtablemodel.cpp \
    ../AccountingMain/AccountDataBase/accdatabase.cpp \
    ../AccountingMain/AccountDataBase/datetransformation.cpp \
    ../AccountingMain/AccountDataBase/csvreader.cpp \
    ../AccountingMain/AccountDataBase/statementextracttablemodel.cpp \
    ../AccountingMain/AccountDataBase/statements.cpp \
    ../AccountingMain/AccountDataBase/statementtablemodel.cpp \
    AccountDataBase/tabletransformer.cpp

HEADERS += \
    ../AccountingMain/AccountDataBase/csvtablemodel.h \
    ../AccountingMain/AccountDataBase/accdatabase.h \
    ../AccountingMain/AccountDataBase/statementrow.h \
    ../AccountingMain/AccountDataBase/statementtablemodel.h \
    ../AccountingMain/AccountDataBase/simpletransformations.h \
    ../AccountingMain/AccountDataBase/datetransformation.h \
    ../AccountingMain/AccountDataBase/transformation.h \
    ../AccountingMain/AccountDataBase/statementextractrow.h \
    ../AccountingMain/AccountDataBase/statementextractrow.h \
    ../AccountingMain/AccountDataBase/csvreader.h \
    ../AccountingMain/AccountDataBase/statementextracttablemodel.h \
    ../AccountingMain/AccountDataBase/statements.h \
    ../AccountingMain/AccountDataBase/statementtablemodel.h \
    AccountDataBase/tabletransformer.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
