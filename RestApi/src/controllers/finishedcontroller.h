#ifndef FINISHEDIPERFCONTROLLER_H
#define FINISHEDIPERFCONTROLLER_H

#include "httprequesthandler.h"
#include "../../global.h"
#include "../../databaseconnection.h"
#include "../../globalnetwork.h"


using namespace stefanfrings;

class FinishedController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(FinishedController)

public:
    FinishedController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    /** запрошенная база данных должна принадлежать вызывающему потоку */
    QSqlDatabase database;

};

#endif // FINISHEDIPERFCONTROLLER_H
