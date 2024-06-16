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

    connect( manager, &IperfManager::iperfChanged, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    timer.start(10000); //10 sec

    if(!manager->startNewProcess(isServer, body["uuid"].toString(), body["command"].toString())){
        QJsonObject object{
            {"IperfManager", "Процесса с таким идентификатором уже существует."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }

    loop.exec();

    if(!timer.isActive()){
        QJsonObject object{
            {"IperfManager", "Процесс не был запущен (не поступил сигнал). Возможна ошибка в команде."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
    }

//    response.setStatus(200, "Ok");
//    response.setHeader("Content-Type", "application/json");
//    QJsonObject object{
//        {"IperfManager", "Процесс успешно запустился."}
//    };

//    response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);

    switch (manager->getProcessStatus(body["uuid"].toString())) {
    case ProcessState::Running:
    {
        QJsonObject object{
            {"IperfManager", "Процесс успешно запустился."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    case ProcessState::Finished:
    {
        QJsonObject object{
            {"IperfManager", "Процесс быстро заврешился. Возможна ошибка в команде."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    case ProcessState::Crashed:
    {
        QJsonObject object{
            {"IperfManager", "Процесс завершился сбоем через некоторое время после успешного запуска."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    case ProcessState::FailedToStart:
    {
        QJsonObject object{
            {"IperfManager", "Не удалось запустить процесс. Либо запущенная программа отсутствует, либо у вас недостаточно прав для ее запуска."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    default:
        QJsonObject object{
            {"IperfManager", "Неизвестая ошибка. Попробуйте запустить еще раз"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        break;
    }
}
