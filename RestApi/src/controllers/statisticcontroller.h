#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H

#include "httprequesthandler.h"
#include "../globalnetwork.h"
#include "../global.h"

class StatisticController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StatisticController)
public:
    StatisticController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    /** запрошенная база данных должна принадлежать вызывающему потоку */
    QSqlDatabase database;
};

#endif // STATISTICCONTROLLER_H
