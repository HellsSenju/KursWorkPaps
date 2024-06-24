#include "statisticcontroller.h"

StatisticController::StatisticController()
{
}

void StatisticController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    if(!req.contains("from") || !req.contains("to")){
        QJsonObject object{
            {"RestApi", "Обязательные поля from, to"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
    else{
        database = QSqlDatabase::addDatabase("QPSQL", "st");
        database.setHostName(db->getHostName());
        database.setDatabaseName(db->getDbName());
        database.setUserName(db->getUser());
        database.setPassword(db->getPassword());

        if(database.open())
            qDebug("NotificationsController : соединение открыто");
        else
            qDebug("NotificationsController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));

        QSqlQuery query = QSqlQuery(database.database("st"));

        QJsonObject res = db->getStatistic(query,
                                           req.value("from").toString(),
                                           req.value("to").toString());

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }
}
