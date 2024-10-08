#ifndef STARTIPERFCONTROLLER_H
#define STARTIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
#include "../../global.h"

using namespace stefanfrings;

class StartIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StartIperfController)

public:
    StartIperfController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    bool checkRequest(QJsonObject request);
};

#endif // STARTIPERFCONTROLLER_H
