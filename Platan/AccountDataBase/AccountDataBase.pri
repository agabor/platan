
SOURCES += \
    ../Platan/AccountDataBase/csvtablemodel.cpp \
    ../Platan/AccountDataBase/accdatabase.cpp \
    ../Platan/AccountDataBase/datetransformation.cpp \
    ../Platan/AccountDataBase/csvreader.cpp \
    ../Platan/AccountDataBase/statementextracttablemodel.cpp \
    ../Platan/AccountDataBase/statements.cpp \
    ../Platan/AccountDataBase/statementtablemodel.cpp \
    AccountDataBase/tabletransformer.cpp \
    AccountDataBase/csvanalyser.cpp

HEADERS += \
    ../Platan/AccountDataBase/csvtablemodel.h \
    ../Platan/AccountDataBase/accdatabase.h \
    ../Platan/AccountDataBase/statementrow.h \
    ../Platan/AccountDataBase/statementtablemodel.h \
    ../Platan/AccountDataBase/simpletransformations.h \
    ../Platan/AccountDataBase/datetransformation.h \
    ../Platan/AccountDataBase/transformation.h \
    ../Platan/AccountDataBase/statementextractrow.h \
    ../Platan/AccountDataBase/statementextractrow.h \
    ../Platan/AccountDataBase/csvreader.h \
    ../Platan/AccountDataBase/statementextracttablemodel.h \
    ../Platan/AccountDataBase/statements.h \
    ../Platan/AccountDataBase/statementtablemodel.h \
    AccountDataBase/tabletransformer.h \
    AccountDataBase/csvanalyser.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
