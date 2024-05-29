TARGET = IperfManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*

SOURCES += \
    src/global.cpp \
    src/requestmapper.cpp \
        src/main.cpp \
        src/iperfserver.cpp \
    src/startcontroller.cpp \
    src/statisticcontroller.cpp \
    src/stopcontroller.cpp \
    src/testcontroller.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    src/global.h \
    src/requestmapper.h \
    src/abstractiperf.h \
    src/iperfserver.h \
    src/startcontroller.h \
    src/statisticcontroller.h \
    src/stopcontroller.h \
    src/testcontroller.h

include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)
