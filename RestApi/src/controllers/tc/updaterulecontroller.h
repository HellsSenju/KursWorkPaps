#ifndef UPDATERULECONTROLLER_H
#define UPDATERULECONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
#include "../../global.h"


using namespace stefanfrings;

class UpdateRuleController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(UpdateRuleController)


public:
    UpdateRuleController();

    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // UPDATERULECONTROLLER_H
