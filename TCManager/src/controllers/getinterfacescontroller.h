#ifndef GETINTERFACESCONTROLLER_H
#define GETINTERFACESCONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"

using namespace stefanfrings;

class GetInterfacesController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(GetInterfacesController)

public:
    GetInterfacesController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // GETINTERFACESCONTROLLER_H
