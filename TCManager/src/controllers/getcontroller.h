#ifndef GETCONTROLLER_H
#define GETCONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"


using namespace stefanfrings;

class GetController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(GetController)

public:
    GetController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // GETCONTROLLER_H
