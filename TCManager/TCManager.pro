TARGET = TCManager
TEMPLATE = app
QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*


SOURCES += \
    src/global.cpp \
    src/controllers/requestmapper.cpp \
    src/controllers/testcontroller.cpp \
    src/main.cpp


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/global.h \
    src/controllers/requestmapper.h \
    src/controllers/startcontroller.h \
    src/controllers/testcontroller.h

include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids
