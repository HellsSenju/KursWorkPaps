#include "statisticfromiperfcontroller.h"
#include <QUuid>

StatisticFromIperfController::StatisticFromIperfController()
{
}

void StatisticFromIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug().noquote() << "StatisticFromIperfController : " << request.getBody();
    QJsonObject req = network->parseRequest(request.getBody());

    QString connection = QUuid::createUuid().toString();
    database = QSqlDatabase::addDatabase("QPSQL", connection);
    database.setHostName(db->getHostName());
    database.setDatabaseName(db->getDbName());
    database.setUserName(db->getUser());
    database.setPassword(db->getPassword());

    if(database.open()){
        qDebug("StatisticFromIperfController : соединение открыто");

        QSqlQuery query = QSqlQuery(database.database(connection));

        QJsonObject res = db->insertStatistic(query, req);

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }
    else{
        qDebug("StatisticFromIperfController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject res{
            {"RestApi", "Не удалось подключиться к бд."}
        };


        response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
    }

    database.removeDatabase(connection);
}
