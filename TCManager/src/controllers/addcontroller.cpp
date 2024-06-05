#include "addcontroller.h"

AddController::AddController()
{
    connect(this, &AddController::addRule, pool, &ProcessesPool::onAddRule);
}

void AddController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server")){
        QJsonObject body = req["server"].toObject();

        emit addRule(body["uuid"].toString(), body["command"].toString());

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
