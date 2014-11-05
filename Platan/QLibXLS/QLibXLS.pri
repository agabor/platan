
SOURCES += \
    ../Platan/QLibXLS/xlsdocument.cpp \
    ../Platan/QLibXLS/qxlsdocument.cpp
	
HEADERS += \
    ../Platan/QLibXLS/xlsdocument.h \
    ../Platan/QLibXLS/qxlsdocument.h
	

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
