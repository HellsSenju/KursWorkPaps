#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class StatisticController : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StatisticController)

public:
    StatisticController();

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // STATISTICCONTROLLER_H
