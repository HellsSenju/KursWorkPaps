#include "notificationscontroller.h"

NotificationsController::NotificationsController()
{
}

void NotificationsController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "NotificationsController : " << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    database = QSqlDatabase::addDatabase("QPSQL", "n");
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open()){
        qDebug("NotificationsController : соединение открыто");

        QSqlQuery query = QSqlQuery(database.database("n"));

        QJsonObject res = db->getNotifications(query, req.value("timestamp").toString());

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);

        database.removeDatabase("n");
    }
    else{
        qDebug("NotificationsController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject res{
            {"RestApi", "Не удалось подключиться к бд."}
        };


        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);

        database.removeDatabase("n");

    }

}
