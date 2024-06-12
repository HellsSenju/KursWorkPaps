#include "startcontroller.h"

StartController::StartController()
{
}

void StartController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(!req.contains("server") && !req.contains("client")){
        //406 Not Acceptable — запрошенный URI не может удовлетворить переданным в заголовке характеристикам.
        //Если метод был не HEAD, то сервер должен вернуть список допустимых характеристик для данного ресурса.
        response.setStatus(400, "Некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"Запросы : ", " тут запросы"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }

    bool isServer;
    QJsonObject body;
    if(req.contains("server")){
        body = req["server"].toObject();
        isServer = true;
    }
    else{
        body = req["client"].toObject();
        isServer = false;
    }

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect( manager, &IperfManager::iperfStarted, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    timer.start(10000); //10 sec

    manager->startNewProcess(isServer, body["uuid"].toString(), body["command"].toString());

    loop.exec();

    bool started = manager->getProcessStartStatus(body["uuid"].toString());
    if(timer.isActive() && started){

        response.setStatus(200, "Ok");
        response.setHeader("Content-Type", "application/json");
        QJsonObject object{
            {"iperf_manager", "Процесс успешно запущен"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }
    else if(timer.isActive() && !started){
        response.setStatus(200, "Ok");
        response.setHeader("Content-Type", "application/json");
        QJsonObject object{
            {"iperf_manager", "Процесс по некоторым причинал не смог запуститься"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }
    else{
        response.setStatus(500, "Internal Server Error");
        response.setHeader("Content-Type", "application/json");
        QJsonObject object{
            {"iperf_manager", "Сигнал от процесса не был получен."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }
}
