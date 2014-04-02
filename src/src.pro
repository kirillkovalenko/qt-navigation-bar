TARGET = navbar
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = ../lib
TARGET = $$qtLibraryTarget($$TARGET)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += navbar.cpp \
    navbarpagelistwidget.cpp \
    navbarsplitter.cpp \
    navbaroptionsdialog.cpp

HEADERS += navbar.h \
    navbarpagelistwidget.h \
    navbarsplitter.h \
    navbaroptionsdialog.h \
    navbarpage.h

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

FORMS += \
    navbaroptionsdialog.ui
