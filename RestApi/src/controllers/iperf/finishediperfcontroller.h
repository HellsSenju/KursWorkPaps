#ifndef FINISHEDIPERFCONTROLLER_H
#define FINISHEDIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../global.h"
#include "../../databaseconnection.h"
#include "../../globalnetwork.h"


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
