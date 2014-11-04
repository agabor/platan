
SOURCES += \
    ../Platan/QLibXLS/xlsdocument.cpp
	
HEADERS += \
    ../Platan/PlatanCSV/xlsdocument.h
	

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
