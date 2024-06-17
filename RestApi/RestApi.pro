TARGET = RestApi
TEMPLATE = app
QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*

SOURCES += \
        src/controllers/addrulecontroller.cpp \
        src/controllers/deleterulecontroller.cpp \
        src/controllers/getrulecontroller.cpp \
        src/controllers/gettcinterfacescontroller.cpp \
        src/controllers/startiperfcontroller.cpp \
        src/controllers/stopiperfcontroller.cpp \
        src/controllers/updaterulecontroller.cpp \
        src/main.cpp\
        src/global.cpp \
        src/controllers/requestmapper.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
        src/controllers/addrulecontroller.h \
        src/controllers/deleterulecontroller.h \
        src/controllers/getrulecontroller.h \
        src/controllers/gettcinterfacescontroller.h \
        src/controllers/startiperfcontroller.h \
        src/controllers/stopiperfcontroller.h \
        src/controllers/updaterulecontroller.h \
        src/global.h \
        src/controllers/requestmapper.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)


# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
