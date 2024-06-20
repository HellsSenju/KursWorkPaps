TARGET = TCManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*


SOURCES += \
    ../IperfManager/src/globalnetwork.cpp \
    ../IperfManager/src/network.cpp \
    src/controllers/addcontroller.cpp \
    src/controllers/deletecontroller.cpp \
    src/controllers/getcontroller.cpp \
    src/controllers/getinterfacescontroller.cpp \
    src/controllers/updatecontroller.cpp \
    src/controllers/requestmapper.cpp \
    src/models/process.cpp \
    src/models/tcprocess.cpp \
    src/processespool.cpp \
    src/global.cpp \
    src/main.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../IperfManager/src/globalnetwork.h \
    ../IperfManager/src/network.h \
    src/controllers/addcontroller.h \
    src/controllers/deletecontroller.h \
    src/controllers/getcontroller.h \
    src/controllers/getinterfacescontroller.h \
    src/controllers/updatecontroller.h \
    src/controllers/requestmapper.h \
    src/models/abstractprocess.h \
    src/models/process.h \
    src/models/tcprocess.h\
    src/global.h \
    src/processespool.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
