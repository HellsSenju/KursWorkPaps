#include "statisticfromiperfcontroller.h"

StatisticFromIperfController::StatisticFromIperfController()
{
    database = QSqlDatabase::addDatabase("QPSQL", "s");
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open())
        qDebug("NotificationsController : соединение открыто");
    else
        qDebug("NotificationsController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
}

void StatisticFromIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug().noquote() << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    QSqlQuery query = QSqlQuery(database.database("f"));

    QJsonObject res = db->insertStatistic(query, req);

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
}
