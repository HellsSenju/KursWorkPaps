#include "startcontroller.h"

StartController::StartController()
{
}

void StartController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

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
        response.setStatus(200, "Ok");
        response.setHeader("Content-Type", "application/json");
        QJsonObject object{
            {"iperf_manager", "Сигнал от процесса не был получен. Процесс не выполнился"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }
}
