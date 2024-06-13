TARGET = TCManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*


SOURCES += \
    src/controllers/addcontroller.cpp \
    src/controllers/getcontroller.cpp \
    src/global.cpp \
    src/controllers/requestmapper.cpp \
    src/controllers/testcontroller.cpp \
    src/processespool.cpp \
    src/models/tcprocess.cpp \
    src/main.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/controllers/addcontroller.h \
    src/controllers/getcontroller.h \
    src/global.h \
    src/controllers/requestmapper.h \
    src/controllers/testcontroller.h \
    src/processespool.h \
    src/models/tcprocess.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
