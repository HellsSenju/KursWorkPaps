#include "notificationscontroller.h"
#include <QUuid>

NotificationsController::NotificationsController()
{
}

void NotificationsController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << "NotificationsController : " << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    if(req.contains("to") && !req.contains("from")){
        QJsonObject res{
            {"RestApi", "Поле to обязательно используется с полем from. Добавте поле from."}
        };

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
        return;
    }

    QString connection = QUuid::createUuid().toString();
    database = QSqlDatabase::addDatabase("QPSQL", connection);
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open()){
        qDebug("NotificationsController : соединение открыто");

        QSqlQuery query = QSqlQuery(database.database(connection));

        QJsonObject res = db->getNotifications(query, req.value("from").toString(), req.value("to").toString());

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }
    else{
        qDebug("NotificationsController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
        QJsonObject res{
            {"RestApi", "Не удалось подключиться к бд."}
        };


        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }

    database.removeDatabase(connection);
}
