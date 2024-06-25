#include "statisticcontroller.h"
#include <QUuid>


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
        QString connection = QUuid::createUuid().toString();
        database = QSqlDatabase::addDatabase("QPSQL", connection);
        database.setHostName(db->getHostName());
        database.setDatabaseName(db->getDbName());
        database.setUserName(db->getUser());
        database.setPassword(db->getPassword());

        if(database.open()){
            qDebug("StatisticController : соединение открыто");
            QSqlQuery query = QSqlQuery(database.database(connection));

            QJsonObject res = db->getStatistic(query,
                                               req.value("from").toString(),
                                               req.value("to").toString());

            response.setStatus(200,"Ok");
            response.setHeader("Content-Type", "application/json");

            response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
        }
        else{
            qDebug("StatisticController : нет соединения. Ошибка %s", qPrintable(database.lastError().text()));
            response.setStatus(200,"Ok");
            response.setHeader("Content-Type", "application/json");

            QJsonObject res{
                {"RestApi", "Не удалось подключиться к бд."}
            };


            response.write(QJsonDocument(res).toJson(QJsonDocument::Compact), true);
        }

        database.removeDatabase(connection);
    }
}
