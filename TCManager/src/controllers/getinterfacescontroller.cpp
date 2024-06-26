#include "getinterfacescontroller.h"

GetInterfacesController::GetInterfacesController()
{

}

void GetInterfacesController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "GetInterfacesController : " << request.getBody();
    QJsonObject body =  network->parseRequest(request.getBody());
    QString uuid = body["uuid"].toString();

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

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

    pool->execute(uuid, Programs::NMCLI, body["command"].toString());

    timer.start(10000); //10 sec
    loop.exec();

    QJsonObject object;

    QString error = pool->getProcessError(uuid);
    if(!error.isEmpty())
        object["error"] = error;

    QString output = pool->getProcessOutput(uuid);
    if(!output.isEmpty())
        object["output"] = output;

    switch (pool->getProcessState(uuid)) {
    case ProcessState::Finished:
    {
        object.insert("TCManager", "Выполнился.");
        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
     }
        break;

    case ProcessState::Crashed:
    {
        object.insert("TCManager", "Процесс завершился сбоем через некоторое время после успешного запуска.");
        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    case ProcessState::FailedToStart:
    {
        object.insert("TCManager", "Не удалось запустить процесс. "
                                   "Либо программа отсутствует, либо недостаточно прав для запуска, либо неверная команда.");
        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
        break;

    default:
        object.insert("TCManager", "Неизвестая ошибка. Попробуйте запустить еще раз.");
        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        break;
    }

    pool->deleteProcess(uuid);
}
