#ifndef FROMIPERFCONTROLLER_H
#define FROMIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
#include "../../global.h"

using namespace stefanfrings;

class FromIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FromIperfController)

public:
    FromIperfController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // FROMIPERFCONTROLLER_H
