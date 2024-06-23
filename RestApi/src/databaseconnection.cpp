#include "databaseconnection.h"

DataBaseConnection::DataBaseConnection(QSettings *settings, QObject *parent)
    : QObject{parent}
{
    this->settings = settings;
    dbName = settings->value("name").toString();
}

DataBaseConnection::~DataBaseConnection()
{
    settings->deleteLater();
}

void DataBaseConnection::connect()
{
    // добавление нового соединения с бд, где первый аргумент - тип драйвера, второй - название соединения
    db = QSqlDatabase::addDatabase("QPSQL", dbName);
    // задание host name для соединения (должно быть задано до открытия соединения) - ip
    db.setHostName(settings->value("host").toString());
    // название базы данных
    db.setDatabaseName(dbName);
    // имя пользователя
    db.setUserName(settings->value("userName").toString());
    //пароль
    db.setPassword(settings->value("password").toString());

    //открытие соединения
    if(db.open()) qDebug("DataBaseConnection : соединение открыто");
    else qDebug("DataBaseConnection : нет соединения. Ошибка %s", qPrintable(db.lastError().text()));
}

QJsonObject DataBaseConnection::get(QString dbName, QString SQL, QJsonArray injections)
{
    // экземпляр sql запроса к определенной бд
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QJsonObject obj;
    // подготовка запроса к исполнению
    if(!q.prepare(SQL)) {
        obj.insert("error", q.lastError().text());
        return obj;
    }

    // добавление в запрос аргументов (для большей безопасности)
    for(int i = 0; i < injections.size(); i++)
    {
        QJsonValue value = injections[i];
        if(value.isString())
            q.addBindValue(value.toString());
        if(value.isDouble())
            q.addBindValue(value.toDouble());
    }

    // выполнение запроса
    if(!q.exec()){
        obj.insert("error", q.lastError().text());
        return obj;
    }

    //забираются данные из ответы
    for(int j = 0; q.next(); j++){
        QJsonObject buff;

        for(int i = 0; i < q.record().count(); i++){
            buff.insert(q.record().fieldName(i), q.record().value(i).toJsonValue());
        }
        obj.insert(QString::number(j), buff);
    }

    return obj;
}

bool DataBaseConnection::insert(QString tableName, QJsonObject injections)
{
    QSqlQuery query = QSqlQuery(db.database(dbName));

    QSqlQuery q = QSqlQuery(db.database(dbName));
    QString SQL = QString("INSERT INTO %1 (id, name, status) VALUES (?, ?, ?)").arg(tableName);

    if(!q.prepare(SQL)) {
        qDebug() << "prepare fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    for(int i = 0; i < injections.size(); i++)
    {
        QJsonObject object = injections[QString::number(i)].toObject();
        for(QString prop : object.keys()){
            if(object[prop].isString())
                q.addBindValue(object[prop].toString());
            else if(object[prop].isDouble())
                q.addBindValue(object[prop].toDouble());
        }
    }

    if(!q.exec()){
        qDebug() << "exec fasle";
        qDebug() << q.lastError().text();
        return false;
    }
    return true;
}

void DataBaseConnection::insertNotification(QString processId,
                                             QString manager,
                                             QString information,
                                             QString error)
{
    QSqlQuery query = QSqlQuery(db.database(dbName));
    QString SQL;

    // no error
    if(error.isEmpty()){
        qDebug() << "no error";
        SQL = QString("INSERT INTO %1 (process_id, information, manager) VALUES (?, ?, ?)")
                .arg("notification_2");
    }
    else{
        SQL = QString("INSERT INTO %1 (process_id, information, error_msg, manager) VALUES (?, ?, ?, ?)")
                .arg("notification_2");
    }

    if(!query.prepare(SQL)) {
        qDebug() << "with error";
        qDebug("DataBaseConnection : Prepare fasle. %s", qPrintable(query.lastError().text()));
        return;
    }

    query.addBindValue(processId);
    query.addBindValue(information);
    if(!error.isEmpty())
        query.addBindValue(error);
    query.addBindValue(manager);

    if(!query.exec()){
        qDebug("Запрос: %s", qPrintable(query.lastQuery()));
        qDebug("Запрос не выполнился. Ошибка: %s", qPrintable(query.lastError().text()));
        return;
    }

    qDebug("Запрос: %s", qPrintable(query.lastQuery()));
}

bool DataBaseConnection::update(QString dbName, QString tableName, QJsonObject values, QString filter)
{
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QString SQL = QString("UPDATE %1 SET ").arg(tableName);

    for(QString prop : values.keys()){
        QString add = QString("%1 = ?").arg(prop);
        if(values.keys().indexOf(prop) != values.size()-1) add += ",";
        SQL += add;
    }

    SQL += " ";
    SQL += filter;

    if(!q.prepare(SQL)){
        qDebug() << "prepare fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    for(QString prop : values.keys()){
        if(values[prop].isString())
            q.addBindValue(values[prop].toString());
        else if(values[prop].isDouble())
            q.addBindValue(values[prop].toDouble());
    }

    if(!q.exec()){
        qDebug() << "exec fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    return true;
}
