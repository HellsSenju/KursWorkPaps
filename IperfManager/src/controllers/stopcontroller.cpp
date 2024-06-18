#include "stopcontroller.h"

StopController::StopController()
{
    connect(this, &StopController::stop, manager, &IperfManager::stopProcess);
}

void StopController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    QString uuid = req.value("uuid").toString();

    response.setStatus(200, "Ok");
    response.setHeader("Content-Type", "application/json");

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect( manager, &IperfManager::iperfChanged, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    if(!manager->checkDublicates(uuid)){
        QJsonObject object{
            {"IperfManager", "Процесса с таким идентификатором не существует."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
        return;
    }

    manager->stopProcess(uuid);

    timer.start(10000); //10 sec
    loop.exec();

    if(!timer.isActive()){
        QJsonObject object{
            {"IperfManager", "Не пришел сигнал от процесса."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
        return;
    }

    switch (manager->getProcessStatus(uuid)) {
    case ProcessState::Finished:
    {
        QJsonObject object{
            {"IperfManager", "Процесс успешно остановлен."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    default:
        QJsonObject object{
            {"IperfManager", "Неизвестная ошибка."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        break;
    }

    manager->deleteProcess(uuid);
}
