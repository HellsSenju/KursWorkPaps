TARGET = IperfManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*

SOURCES += \
        src/main.cpp \
        src/iperfserver.cpp \

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    src/abstractiperf.h \
    src/iperfserver.h \

include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)
