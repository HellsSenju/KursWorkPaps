#ifndef GETTCINTERFACESCONTROLLER_H
#define GETTCINTERFACESCONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
#include "../../global.h"

using namespace stefanfrings;

class GetTcInterfacesController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(GetTcInterfacesController)

public:
    GetTcInterfacesController();

    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // GETTCINTERFACESCONTROLLER_H
