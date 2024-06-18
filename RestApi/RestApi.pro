TARGET = RestApi
TEMPLATE = app
QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*

SOURCES += \
        src/controllers/iperf/fromiperfcontroller.cpp \
        src/controllers/tc/addrulecontroller.cpp \
        src/controllers/tc/deleterulecontroller.cpp \
        src/controllers/tc/getrulecontroller.cpp \
        src/controllers/tc/gettcinterfacescontroller.cpp \
        src/controllers/tc/updaterulecontroller.cpp \
        src/controllers/iperf/startiperfcontroller.cpp \
        src/controllers/iperf/stopiperfcontroller.cpp \
        src/controllers/requestmapper.cpp \
        src/main.cpp\
        src/global.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
        src/controllers/iperf/fromiperfcontroller.h \
        src/controllers/tc/addrulecontroller.h \
        src/controllers/tc/deleterulecontroller.h \
        src/controllers/tc/getrulecontroller.h \
        src/controllers/tc/gettcinterfacescontroller.h \
        src/controllers/tc/updaterulecontroller.h \
        src/controllers/iperf/startiperfcontroller.h \
        src/controllers/iperf/stopiperfcontroller.h \
        src/controllers/requestmapper.h \
        src/global.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)


# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
