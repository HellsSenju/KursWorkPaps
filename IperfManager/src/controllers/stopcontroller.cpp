#include "stopcontroller.h"

StopController::StopController()
{
    connect(this, &StopController::stop, manager, &IperfManager::stopProcess);
}

void StopController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server")){
        QJsonObject body = req["server"].toObject();

        emit stop(body["uuid"].toString());

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

//        QJsonObject object{
//            {"response", "ляяяяяяяяяяя работает"}
//        };

//        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
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
