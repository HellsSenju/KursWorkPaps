#include "finishediperfcontroller.h"

#include <QUuid>

FinishedController::FinishedController()
{
}

void FinishedController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug().noquote() << "FinishedIperfController : " << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    QString connection = QUuid::createUuid().toString();
    database = QSqlDatabase::addDatabase("QPSQL", connection);
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open()){
        qDebug("FinishedIperfController : соединение открыто");

        QSqlQuery query = QSqlQuery(database.database(connection));
        QJsonObject res = db->insertNotification(query, req);

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }
    else{
        qDebug("FinishedIperfController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject res{
            {"RestApi", "Не удалось подключиться к бд."}
        };


        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);


    }

    database.removeDatabase(connection);
}
