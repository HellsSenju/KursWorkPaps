#include "updatecontroller.h"

UpdateController::UpdateController()
{

}

void UpdateController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject body =  parseRequest(request.getBody());
    QString uuid = body["uuid"].toString();

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect( pool, &ProcessesPool::executed, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    if(pool->checkDublicates(uuid)){
        QJsonObject object{
            {"TCManager", "Процесс с таким идентификатором уже существует."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact),true);
        return;
    }

    timer.start(10000); //10 sec

    pool->execute(uuid, Programs::TC, body["command"].toString());

    loop.exec();

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    switch (pool->getProcessState(uuid)) {
    case ProcessState::Finished:
    {
        QJsonObject object{
            {"TCManager", pool->getProcessOutput(uuid)}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    case ProcessState::Crashed:
    {
        QJsonObject object{
            {"TCManager", "Crashed"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    case ProcessState::FailedToStart:
    {
        QJsonObject object{
            {"TCManager", "FailedToStart"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    default:
        QJsonObject object{
            {"TCManager", "default"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        break;
    }

    pool->deleteProcess(uuid);
}
