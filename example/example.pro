QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app

SOURCES += main.cpp\
        wnd.cpp

HEADERS += wnd.h

RESOURCES += \
    example.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lnavbar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lnavbard
else:unix: LIBS += -L$$OUT_PWD/../lib/ -lnavbar

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../lib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbar.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbard.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lib/libnavbar.a
