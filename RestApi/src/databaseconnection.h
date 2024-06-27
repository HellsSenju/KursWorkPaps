#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

class DataBaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseConnection(const QString &helpersFile, QSettings* settings, QObject *parent = nullptr);
    ~DataBaseConnection();

    /** Создание нового соединения к базе данных*/
    void connect();

    /** Получение новых уведомлений
     *  (если timestamp не указан, берутся уведомления от послденего обращения)*/
    QJsonObject getNotifications(QSqlQuery query,
                                 QString from = "",
                                 QString to = "");

    /** Получение статистических данных*/
    QJsonObject getStatistic(QSqlQuery query, QString from, QString to);

    /** Добавление записи уведомления в базу данных*/
    QJsonObject insertNotification(QSqlQuery query, QJsonObject data);

    /** Добавление записи статистики в базу данных*/
    QJsonObject insertStatistic(QSqlQuery query, QJsonObject data);

    const QString &getDbName() const;
    const QString &getHostName() const;
    const QString &getUser() const;
    const QString &getPassword() const;

private:
    QSqlDatabase db;
    QSettings* settings;
    QString helperFile;

    QString dbName;
    QString hostName;
    QString user;
    QString password;

    QString getLastTime();
    void setNewLastTime(QString newTime);

signals:

};

#endif // DATABASECONNECTION_H
