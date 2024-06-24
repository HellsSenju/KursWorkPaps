#ifndef NOTIFICATIONSCONTROLLER_H
#define NOTIFICATIONSCONTROLLER_H

#include "httprequesthandler.h"
#include "../globalnetwork.h"
#include "../global.h"

class NotificationsController: public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(NotificationsController)

public:
    NotificationsController();

    void service(HttpRequest& request, HttpResponse& response);

private:
    /** запрошенная база данных должна принадлежать вызывающему потоку */
    QSqlDatabase database;

};

#endif // NOTIFICATIONSCONTROLLER_H
