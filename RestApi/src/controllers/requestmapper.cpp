#include "../global.h"
#include "requestmapper.h"
#include "startiperfcontroller.h"
#include "stopiperfcontroller.h"
#include "addrulecontroller.h"
#include "getrulecontroller.h"
#include "gettcinterfacescontroller.h"
#include "updaterulecontroller.h"
#include "deleterulecontroller.h"

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


    if (path.startsWith("/iperf/start"))
        StartIperfController().service(request, response);

    else if (path.startsWith("/iperf/stop"))
        StopIperfController().service(request, response);

    else if (path.startsWith("/tc/get"))
        GetRuleController().service(request, response);

    else if (path.startsWith("/tc/update"))
        UpdateRuleController().service(request, response);

    else if (path.startsWith("/tc/add"))
        AddRuleController().service(request, response);

    else if (path.startsWith("/tc/delete"))
        DeleteRuleController().service(request, response);

    else if (path.startsWith("/tc/interface"))
        GetTcInterfacesController().service(request, response);

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
