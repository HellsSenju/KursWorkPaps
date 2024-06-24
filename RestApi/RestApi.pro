TARGET = RestApi
TEMPLATE = app
QT -= gui
QT += core network sql xml

CONFIG += c++11 console
CONFIG -= app_bundle

OTHER_FILES += etc/* logs/*

SOURCES += \
        src/controllers/iperf/finishediperfcontroller.cpp \
        src/controllers/iperf/statisticfromiperfcontroller.cpp \
        src/controllers/notificationscontroller.cpp \
        src/controllers/tc/addrulecontroller.cpp \
        src/controllers/tc/deleterulecontroller.cpp \
        src/controllers/tc/getrulecontroller.cpp \
        src/controllers/tc/gettcinterfacescontroller.cpp \
        src/controllers/tc/updaterulecontroller.cpp \
        src/controllers/iperf/startiperfcontroller.cpp \
        src/controllers/iperf/stopiperfcontroller.cpp \
        src/controllers/requestmapper.cpp \
        src/databaseconnection.cpp \
        src/globalnetwork.cpp \
        src/main.cpp\
        src/global.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
        src/controllers/iperf/finishediperfcontroller.h \
        src/controllers/iperf/statisticfromiperfcontroller.h \
        src/controllers/notificationscontroller.h \
        src/controllers/tc/addrulecontroller.h \
        src/controllers/tc/deleterulecontroller.h \
        src/controllers/tc/getrulecontroller.h \
        src/controllers/tc/gettcinterfacescontroller.h \
        src/controllers/tc/updaterulecontroller.h \
        src/controllers/iperf/startiperfcontroller.h \
        src/controllers/iperf/stopiperfcontroller.h \
        src/controllers/requestmapper.h \
        src/databaseconnection.h \
        src/global.h \
        src/globalnetwork.h

include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/logging/logging.pri)


# Not used: include(QtWebApp/templateengine/templateengine.pri)

DISTFILES += \
    logs/uuids

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../build/ NetworkLibDebug/release/' -lNetworkLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../build/ NetworkLibDebug/debug/' -lNetworkLib
else:unix: LIBS += -L$$PWD/'../build/ NetworkLibDebug/' -lNetworkLib

INCLUDEPATH += $$PWD/../NetworkLib
DEPENDPATH += $$PWD/../NetworkLib
