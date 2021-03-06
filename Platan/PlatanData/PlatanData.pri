
SOURCES += \
    ../Platan/PlatanData/statements.cpp \
    ../Platan/PlatanData/statementtablemodel.cpp \
    ../Platan/PlatanData/rule.cpp \
    ../Platan/PlatanData/statement.cpp \
    ../Platan/PlatanData/countrycodes.cpp \
    ../Platan/PlatanData/rulemapper.cpp \
    ../Platan/PlatanData/statementmapper.cpp \
    ../Platan/PlatanData/countrydata.cpp \
    ../Platan/PlatanData/countrymapper.cpp \
    ../Platan/PlatanData/rules.cpp \
    ../Platan/PlatanData/viewmodel.cpp \
    ../Platan/PlatanData/applicationdb.cpp \
    ../Platan/PlatanData/projectdb.cpp \
    ../Platan/PlatanData/ruletablemodel.cpp \
    ../Platan/PlatanData/pluginengine.cpp \
    $$PWD/exportrules.cpp \
    $$PWD/jsobject.cpp \
    $$PWD/jsparameters.cpp \
    $$PWD/jslogger.cpp \
    $$PWD/jsapi.cpp \
    $$PWD/networkhandler.cpp \
    $$PWD/jsnetwork.cpp

HEADERS += \
    ../Platan/PlatanData/statementtablemodel.h \
    ../Platan/PlatanData/statements.h \
    ../Platan/PlatanData/statementtablemodel.h \
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
    ../Platan/PlatanData/pluginengine.h \
    $$PWD/exportrules.h \
    $$PWD/jsobject.h \
    $$PWD/jsparameters.h \
    $$PWD/jslogger.h \
    $$PWD/jsapi.h \
    $$PWD/networkhandler.h \
    $$PWD/jsnetwork.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
