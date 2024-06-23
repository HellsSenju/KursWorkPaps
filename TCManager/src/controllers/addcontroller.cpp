#include "addcontroller.h"

AddController::AddController()
{
}

void AddController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject body =  network->parseRequest(request.getBody());
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
            {"TCManager", "Finished"}
        };
        QString error = pool->getProcessError(uuid);
        if(!error.isEmpty())
            object["Error"] = error;

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    case ProcessState::Crashed:
    {
        QJsonObject object{
            {"TCManager", "Процесс завершился сбоем через некоторое время после успешного запуска."}
        };

        QString error = pool->getProcessError(uuid);
        if(!error.isEmpty())
            object["Error"] = error;

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    case ProcessState::FailedToStart:
    {
        QJsonObject object{
            {"TCManager", "Не удалось запустить процесс. "
                "Либо программа отсутствует, либо недостаточно прав для запуска, либо неверная команда."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    default:
        QJsonObject object{
            {"TCManager", "Неизвестая ошибка. Попробуйте запустить еще раз"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        break;
    }

    pool->deleteProcess(uuid);
}
