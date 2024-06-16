#ifndef ADDCONTROLLER_H
#define ADDCONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"

using namespace stefanfrings;

class AddController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(AddController)

public:
    AddController();
    void service(HttpRequest& request, HttpResponse& response);

};

#endif // ADDCONTROLLER_H
