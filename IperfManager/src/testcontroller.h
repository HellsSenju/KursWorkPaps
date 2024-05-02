#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class TestController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(TestController)

public:
    TestController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    QJsonObject parseRequest(const QString& in);
};

#endif // TESTCONTROLLER_H
