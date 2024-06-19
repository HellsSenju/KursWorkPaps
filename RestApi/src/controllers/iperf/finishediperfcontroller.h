#ifndef FINISHEDIPERFCONTROLLER_H
#define FINISHEDIPERFCONTROLLER_H

#include "httprequesthandler.h"


using namespace stefanfrings;

class FinishedIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FinishedIperfController)

public:
    FinishedIperfController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // FINISHEDIPERFCONTROLLER_H
