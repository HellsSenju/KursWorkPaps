#include "startcontroller.h"

StartController::StartController()
{
}

void StartController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    response.setStatus(200, "Ok");
    response.setHeader("Content-Type", "application/json");

    bool isServer;

    if(req.value("mode").toString() == "-s")
        isServer = true;
    else
        isServer = false;

    QString uuid = req.value("uuid").toString();

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect( manager, &IperfManager::iperfChanged, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    if(manager->checkDublicates(uuid)){
        QJsonObject object{
            {"IperfManager", "Процесс с таким идентификатором уже существует."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
        return;
    }

    manager->startNewProcess(isServer, uuid, req.value("command").toString());

    timer.start(10000); //10 sec
    loop.exec();

    if(!timer.isActive()){
        QJsonObject object{
            {"IperfManager", "Процесс не был запущен (не поступил сигнал). Возможна ошибка в команде."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
        return;
    }

    switch (manager->getProcessStatus(uuid)) {
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
            {"IperfManager", "Не удалось запустить процесс. "
                "Либо программа отсутствует, либо недостаточно прав для запуска, либо неверная команда."}
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
