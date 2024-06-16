#ifndef DELETECONTROLLER_H
#define DELETECONTROLLER_H

#include "httprequesthandler.h"
#include "../global.h"

using namespace stefanfrings;

class DeleteController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(DeleteController)

public:
    DeleteController();
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // DELETECONTROLLER_H
