#include "finishediperfcontroller.h"

FinishedIperfController::FinishedIperfController()
{
}

void FinishedIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug().noquote() << "FinishedIperfController : " << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    database = QSqlDatabase::addDatabase("QPSQL", "f");
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open()){
        qDebug("FinishedIperfController : соединение открыто");
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



        QSqlQuery query = QSqlQuery(database.database("f"));
        QJsonObject res = db->insertNotification(query, uuid, from, msg, error);

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
        database.removeDatabase("f");
    }
    else{
        qDebug("FinishedIperfController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject res{
            {"RestApi", "Не удалось подключиться к бд."}
        };


        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);

        database.removeDatabase("f");
    }


}
