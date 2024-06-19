#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

class DataBaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseConnection(QSettings* settings, QObject *parent = nullptr);

    void connect();
    QJsonObject get(QString dbName, QString SQL, QJsonArray injections);
    bool insert(QString dbName, QString tableName, QJsonObject injections);
    bool update(QString dbName, QString tableName, QJsonObject values, QString filter);

private:
    QSqlDatabase db;
    QSettings* settings;

signals:

};

#endif // DATABASECONNECTION_H
