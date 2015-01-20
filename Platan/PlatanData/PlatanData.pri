
SOURCES += \
    ../Platan/PlatanData/datetransformation.cpp \
    ../Platan/PlatanData/statements.cpp \
    ../Platan/PlatanData/statementtablemodel.cpp \
    ../Platan/PlatanData/tabletransformer.cpp \
    ../Platan/PlatanData/rule.cpp \
    ../Platan/PlatanData/statement.cpp \
    ../Platan/PlatanData/countrycodes.cpp \
    ../Platan/PlatanData/rulemapper.cpp \
    ../Platan/PlatanData/statementmapper.cpp \
    ../Platan/PlatanData/countrydata.cpp \
    ../Platan/PlatanData/countrymapper.cpp \
    ../Platan/PlatanData/simpletransformations.cpp \
    ../Platan/PlatanData/transformation.cpp \
    ../Platan/PlatanData/rules.cpp \
    ../Platan/PlatanData/viewmodel.cpp \
    ../Platan/PlatanData/applicationdb.cpp \
    ../Platan/PlatanData/projectdb.cpp \
    ../Platan/PlatanData/ruletablemodel.cpp \
    $$PWD/exportrules.cpp

HEADERS += \
    ../Platan/PlatanData/statementtablemodel.h \
    ../Platan/PlatanData/simpletransformations.h \
    ../Platan/PlatanData/datetransformation.h \
    ../Platan/PlatanData/transformation.h \
    ../Platan/PlatanData/statements.h \
    ../Platan/PlatanData/statementtablemodel.h \
    ../Platan/PlatanData/tabletransformer.h \
    ../Platan/PlatanData/rule.h \
    ../Platan/PlatanData/statement.h \
    ../Platan/PlatanData/countrycodes.h \
    ../Platan/PlatanData/rulemapper.h \
    ../Platan/PlatanData/statementmapper.h \
    ../Platan/PlatanData/countrydata.h \
    ../Platan/PlatanData/country.h \
    ../Platan/PlatanData/countrymapper.h \
    ../Platan/PlatanData/rules.h \
    ../Platan/PlatanData/viewmodel.h \
    ../Platan/PlatanData/applicationdb.h \
    ../Platan/PlatanData/projectdb.h \
    ../Platan/PlatanData/ruletablemodel.h \
    $$PWD/exportrules.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
