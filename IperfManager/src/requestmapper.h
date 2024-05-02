#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler
{
    Q_OBJECT
    Q_DISABLE_COPY(RequestMapper)
public:
    RequestMapper(QObject* parent=0);

    ~RequestMapper();
    /**
     * @brief Распределяет приходящие HTTP запросы по котроллерам в зависимости от URL
     * @param request
     * @param response
     */
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // REQUESTMAPPER_H
