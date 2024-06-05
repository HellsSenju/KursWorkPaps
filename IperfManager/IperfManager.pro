TARGET = IperfManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/* otherFiles/*

SOURCES += \
    src/global.cpp \
    src/iperfmanager.cpp \
    src/controllers/requestmapper.cpp \
    src/main.cpp \
    src/models/iperfclient.cpp \
    src/models/iperfserver.cpp \
    src/controllers/startcontroller.cpp \
    src/controllers/statisticcontroller.cpp \
    src/controllers/stopcontroller.cpp \
    src/controllers/testcontroller.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    src/global.h \
    src/iperfmanager.h \
    src/controllers/requestmapper.h \
    src/models/abstractiperf.h \
    src/models/iperfclient.h \
    src/models/iperfserver.h \
    src/controllers/startcontroller.h \
    src/controllers/statisticcontroller.h \
    src/controllers/stopcontroller.h \
    src/controllers/testcontroller.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
