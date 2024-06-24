#ifndef STATISTICFROMIPERFCONTROLLER_H
#define STATISTICFROMIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../global.h"
#include "../../databaseconnection.h"
#include "../../globalnetwork.h"


using namespace stefanfrings;

class StatisticFromIperfController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(StatisticFromIperfController)

public:
    StatisticFromIperfController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    /** запрошенная база данных должна принадлежать вызывающему потоку */
    QSqlDatabase database;
};

#endif // STATISTICFROMIPERFCONTROLLER_H
