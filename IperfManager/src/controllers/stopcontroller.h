#ifndef STOPCONTROLLER_H
#define STOPCONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"


using namespace stefanfrings;

class StopController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StopController)

public:
    StopController();

    void service(HttpRequest& request, HttpResponse& response);

signals:
    void stop(const QString &uuid);
};

#endif // STOPCONTROLLER_H
