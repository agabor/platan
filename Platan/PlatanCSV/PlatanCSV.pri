
SOURCES += \
    ../Platan/PlatanCSV/csvtablemodel.cpp \
    ../Platan/PlatanCSV/csvreader.cpp \
    ../Platan/PlatanCSV/csvanalyser.cpp \
    ../Platan/PlatanCSV/csvconfigwidget.cpp \
    ../Platan/PlatanCSV/csvpropertieswidget.cpp \
    ../Platan/PlatanCSV/tablehelpers.cpp

HEADERS += \
    ../Platan/PlatanCSV/csvtablemodel.h \
    ../Platan/PlatanCSV/csvreader.h \
    ../Platan/PlatanCSV/csvanalyser.h \
    ../Platan/PlatanCSV/csvconfigwidget.h \
    ../Platan/PlatanCSV/csvpropertieswidget.h \
    ../Platan/PlatanCSV/tablehelpers.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
