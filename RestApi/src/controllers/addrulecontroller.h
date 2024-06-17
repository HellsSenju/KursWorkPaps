#ifndef ADDRULECONTROLLER_H
#define ADDRULECONTROLLER_H

#include "httprequesthandler.h"


using namespace stefanfrings;

class AddRuleController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(AddRuleController)

public:
    AddRuleController();

    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // ADDRULECONTROLLER_H
