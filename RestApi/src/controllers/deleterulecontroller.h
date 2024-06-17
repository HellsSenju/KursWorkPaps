#ifndef DELETERULECONTROLLER_H
#define DELETERULECONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class DeleteRuleController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(DeleteRuleController)


public:
    DeleteRuleController();
    void service(HttpRequest& request, HttpResponse& response);

private:

    bool checkRequest(QJsonObject request);
};

#endif // DELETERULECONTROLLER_H
