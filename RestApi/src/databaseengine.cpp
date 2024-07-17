#include "databaseengine.h"

DatabaseEngine::DatabaseEngine(QSettings *settings, int poolSize, QObject *parent)
    : QObject{parent}
{
    if(poolSize <= 0) poolSize = 10;
    this->poolSize = poolSize;

    dbName = settings->value("name").toString();
    hostName= settings->value("host").toString();
    user= settings->value("userName").toString();
    password= settings->value("password").toString();

    configureConnections();
}

void DatabaseEngine::configureConnections()
{
    for(int i = 0; i < poolSize; i++){
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", QString::number(i));
        db.setHostName(hostName);
        db.setDatabaseName(dbName);
        db.setUserName(user);
        db.setPassword(password);
    }
}
