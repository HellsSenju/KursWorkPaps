#include "notificationscontroller.h"

NotificationsController::NotificationsController()
{
    if(!database.isOpen()){
        database = QSqlDatabase::addDatabase("QPSQL", "n");
        database.setHostName(db->getHostName());
        database.setDatabaseName(db->getDbName());
        database.setUserName(db->getUser());
        database.setPassword(db->getPassword());

        if(database.open())
            qDebug("NotificationsController : соединение открыто");
        else
            qDebug("NotificationsController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
    }
}

void NotificationsController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    QSqlQuery query = QSqlQuery(database.database("n"));

    QJsonObject res = db->getNotifications(query, req.value("timestamp").toString());

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
}
