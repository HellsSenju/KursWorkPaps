TARGET = IperfManager
TEMPLATE = app
QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/* otherFiles/*

SOURCES += \
    src/global.cpp \
    src/globalnetwork.cpp \
    src/iperfmanager.cpp \
    src/main.cpp \
    src/models/iperfclient.cpp \
    src/models/iperfserver.cpp \
    src/controllers/requestmapper.cpp \
    src/controllers/startcontroller.cpp \
    src/controllers/stopcontroller.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    src/global.h \
    src/globalnetwork.h \
    src/iperfmanager.h \
    src/models/abstractiperf.h \
    src/models/iperfclient.h \
    src/models/iperfserver.h \
    src/controllers/requestmapper.h \
    src/controllers/startcontroller.h \
    src/controllers/stopcontroller.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)

# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/NetworkDebug/release/ -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/NetworkDebug/debug/ -lNetwork
else:unix: LIBS += -L$$PWD/../build/NetworkDebug/ -lNetwork

INCLUDEPATH += $$PWD/../Network
DEPENDPATH += $$PWD/../Network

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build/NetworkDebug/release/libNetwork.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build/NetworkDebug/debug/libNetwork.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build/NetworkDebug/release/Network.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build/NetworkDebug/debug/Network.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build/NetworkDebug/libNetwork.a
