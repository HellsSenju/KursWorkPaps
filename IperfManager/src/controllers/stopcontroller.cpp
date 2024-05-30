#include "stopcontroller.h"

StopController::StopController()
{

}

void StopController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server")){
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"response", "ляяяяяяяяяяя работает"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }

    else{
        response.setStatus(400, "неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"response", "ляяяяяяяяяяя работает - 400"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}
