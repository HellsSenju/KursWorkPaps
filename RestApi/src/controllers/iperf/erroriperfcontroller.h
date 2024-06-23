#ifndef ERRORIPERFCONTROLLER_H
#define ERRORIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
#include "../../global.h"

using namespace stefanfrings;

class ErrorIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(ErrorIperfController)

public:
    ErrorIperfController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // ERRORIPERFCONTROLLER_H
