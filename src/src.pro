TARGET = navbar
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = ../lib
TARGET = $$qtLibraryTarget($$TARGET)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += navbar.cpp \
    navbarpagelist.cpp \
    navbarsplitter.cpp

HEADERS += navbar.h \
    navbarpagelist.h \
    navbarsplitter.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

RESOURCES += \
    navbar.qrc
