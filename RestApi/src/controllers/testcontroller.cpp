#include "testcontroller.h"
#include "glob.h"
#include <QJsonObject>
#include <QJsonDocument>

TestController::TestController()
{

}

void TestController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server") || req.contains("client")){
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "Запрос принят и передан дальше"}
        };


    }

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
