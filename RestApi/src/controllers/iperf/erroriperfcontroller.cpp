#include "erroriperfcontroller.h"

ErrorIperfController::ErrorIperfController()
{

}

void ErrorIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  network->parseRequest(request.getBody());
    QString uuid = req.value("uuid").toString();
    QString error = "";
    if(req.contains("error"))
        error = req.value("error").toString();

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    QJsonObject object{
        {"RestApi", "Данные получены"}
    };

    response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
}
