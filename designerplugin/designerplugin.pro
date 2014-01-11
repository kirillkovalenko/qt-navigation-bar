TARGET = navbarplugin
TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += designer
lessThan(QT_MAJOR_VERSION, 5): CONFIG  += designer plugin

TARGET = $$qtLibraryTarget($$TARGET)
DESTDIR = $$[QT_INSTALL_PLUGINS]/designer

SOURCES += \
    navbarplugin.cpp \
    navbarcontainerextension.cpp \
    navbarextensionfactory.cpp

HEADERS += \
    navbarplugin.h \
    navbarcontainerextension.h \
    navbarextensionfactory.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lnavbar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lnavbard
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lnavbar

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../lib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbar.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbard.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbar.a


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
