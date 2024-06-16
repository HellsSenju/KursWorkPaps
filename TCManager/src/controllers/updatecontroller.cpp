#include "updatecontroller.h"

UpdateController::UpdateController()
{

}

void UpdateController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject body =  parseRequest(request.getBody());


    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect( pool, &ProcessesPool::executed, &loop, &QEventLoop::quit);
    connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

    timer.start(10000); //10 sec

    pool->execute(body["uuid"].toString(), Programs::TC, body["command"].toString());

    loop.exec();

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    switch (pool->getProcessState(body["uuid"].toString())) {
    case ProcessState::Finished:
    {
        QJsonObject object{
            {"TCManager", "Finished"}
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
}
