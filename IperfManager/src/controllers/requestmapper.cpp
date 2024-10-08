#include "requestmapper.h"
#include "../global.h"
#include "startcontroller.h"
#include "stopcontroller.h"
#include "statisticcontroller.h"

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
}

RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}

void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

//    HttpSession session = sessionStore->getSession(request, response);
//    qDebug("RequestMapper: sessionId = %s", session.getId().data());

    if (path.startsWith("/start"))
    {
        StartController().service(request, response);
    }

    else if (path.startsWith("/stop"))
    {
        StopController().service(request, response);
    }
    // All other pathes are mapped to the static file controller.
    // In this case, a single instance is used for multiple requests.
    else
    {
        staticFileController->service(request, response);
    }

    qDebug("RequestMapper: finished request");

    // Clear the log buffer
    if (logger)
    {
       logger->clear();
    }
}
