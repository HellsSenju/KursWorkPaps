#include "finishediperfcontroller.h"

FinishedIperfController::FinishedIperfController()
{

}

void FinishedIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    QJsonObject object{
        {"RestApi", "Данные получены"}
    };

    response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
}
