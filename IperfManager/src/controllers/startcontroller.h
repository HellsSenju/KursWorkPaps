#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"

#include <QObject>

using namespace stefanfrings;

class StartController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StartController)

public:
    StartController();

    void service(HttpRequest& request, HttpResponse& response);

signals:
    void start(bool server, const QString &uuid, const QString &command);
};

#endif // STARTCONTROLLER_H
