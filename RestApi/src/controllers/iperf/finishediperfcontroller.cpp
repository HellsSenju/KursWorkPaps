#include "finishediperfcontroller.h"

FinishedIperfController::FinishedIperfController()
{
    connect(this, &FinishedIperfController::insert,
            db, &DataBaseConnection::insertNotification);
}

void FinishedIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug().noquote() << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    QString uuid = req.value("uuid").toString();
    QString from = req.value("from").toString();
    int exitStatus = req.value("exitStatus").toInt();
    int exitCode = req.value("exitCode").toInt();
    QString error = req.contains("error") ? req.value("error").toString() : "";

    QString msg = "";

    if(exitStatus == 0)
        msg.append("Процесс завершился (Normal exit). ");
    else
        msg.append(QString("Процесс завершился с ошибкой = %s (Crashed exit). ").arg(exitCode));

    if(exitCode == 0)
        msg.append("Выполнено без ошибок");
    if(exitCode == 1)
        msg.append("Общая ощибка.");
    if(exitCode == 2)
        msg.append("Неправильное использование команды или аргумента");

    emit insert(uuid, from, msg, error);

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    QJsonObject object{
        {"RestApi", "Данные получены"}
    };

    response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
}
