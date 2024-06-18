#ifndef STARTIPERFCONTROLLER_H
#define STARTIPERFCONTROLLER_H

#include "httprequesthandler.h"


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
    QString configureParams(QJsonObject request);
};

#endif // STARTIPERFCONTROLLER_H
