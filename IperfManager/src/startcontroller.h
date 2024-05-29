#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class StartController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StartController)

public:
    StartController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // STARTCONTROLLER_H
