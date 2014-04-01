
SOURCES += \
    AccountDataBase/csvtablemodel.cpp \
    AccountDataBase/accountdata.cpp \
    AccountDataBase/accdatabase.cpp \
    AccountDataBase/datetransformation.cpp

HEADERS += \
    AccountDataBase/csvtablemodel.h \
    AccountDataBase/accountdata.h \
    AccountDataBase/accdatabase.h \
    AccountDataBase/statementrow.h \
    AccountDataBase/simpletransformations.h \
    AccountDataBase/datetransformation.h \
    AccountDataBase/transformation.h \
    AccountDataBase/statementextractrow.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
