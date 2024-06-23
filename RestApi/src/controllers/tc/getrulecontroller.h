#ifndef GETRULECONTROLLER_H
#define GETRULECONTROLLER_H

#include "httprequesthandler.h"
#include "../../globalnetwork.h"
//#include "../../global.h"

using namespace stefanfrings;

class GetRuleController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(GetRuleController)


public:
    GetRuleController();

    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // GETRULECONTROLLER_H
