
SOURCES += \
    ../Platan/PlatanData/csvtablemodel.cpp \
    ../Platan/PlatanData/datetransformation.cpp \
    ../Platan/PlatanData/csvreader.cpp \
    ../Platan/PlatanData/statements.cpp \
    ../Platan/PlatanData/statementtablemodel.cpp \
    ../Platan/PlatanData/tabletransformer.cpp \
    ../Platan/PlatanData/csvanalyser.cpp \
    ../Platan/PlatanData/rule.cpp \
    ../Platan/PlatanData/statement.cpp \
    PlatanData/country.cpp

HEADERS += \
    ../Platan/PlatanData/csvtablemodel.h \
    ../Platan/PlatanData/statementrow.h \
    ../Platan/PlatanData/statementtablemodel.h \
    ../Platan/PlatanData/simpletransformations.h \
    ../Platan/PlatanData/datetransformation.h \
    ../Platan/PlatanData/transformation.h \
    ../Platan/PlatanData/csvreader.h \
    ../Platan/PlatanData/statements.h \
    ../Platan/PlatanData/statementtablemodel.h \
    ../Platan/PlatanData/tabletransformer.h \
    ../Platan/PlatanData/csvanalyser.h \
    ../Platan/PlatanData/rule.h \
    ../Platan/PlatanData/statement.h \
    PlatanData/country.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
