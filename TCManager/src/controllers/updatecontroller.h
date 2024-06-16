#ifndef UPDATECONTROLLER_H
#define UPDATECONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"

using namespace stefanfrings;

class UpdateController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(UpdateController)

public:
    UpdateController();
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // UPDATECONTROLLER_H
