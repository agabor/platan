
SOURCES += \
    AccountDataBase/csvtablemodel.cpp \
    AccountDataBase/accdatabase.cpp \
    AccountDataBase/datetransformation.cpp \
    AccountDataBase/csvreader.cpp

HEADERS += \
    AccountDataBase/csvtablemodel.h \
    AccountDataBase/accdatabase.h \
    AccountDataBase/statementrow.h \
    AccountDataBase/simpletransformations.h \
    AccountDataBase/datetransformation.h \
    AccountDataBase/transformation.h \
    AccountDataBase/statementextractrow.h \
    AccountDataBase/csvreader.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
