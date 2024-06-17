#ifndef STOPIPERFCONTROLLER_H
#define STOPIPERFCONTROLLER_H

#include "httprequesthandler.h"


using namespace stefanfrings;

class StopIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StopIperfController)
public:
    StopIperfController();

    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // STOPIPERFCONTROLLER_H
