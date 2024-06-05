#include "startcontroller.h"

StartController::StartController()
{
    connect(this, &StartController::start, manager, &IperfManager::startNewProcess);
}

void StartController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());


    if(req.contains("server")){
        QJsonObject body = req["server"].toObject();

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        emit start(true, body["uuid"].toString(), body["command"].toString());
    }

    else if(req.contains("client")){
        QJsonObject body = req["server"].toObject();

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");
    }

    else{
        //406 Not Acceptable — запрошенный URI не может удовлетворить переданным в заголовке характеристикам.
        //Если метод был не HEAD, то сервер должен вернуть список допустимых характеристик для данного ресурса.
        response.setStatus(400, "Некорректный запрос");
        response.setHeader("Content-Type", "application/json");
    }
}
