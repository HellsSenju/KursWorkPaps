#include "databaseconnection.h"

DataBaseConnection::DataBaseConnection(const QString &helpersFile, QSettings *settings, QObject *parent)
    : QObject{parent}
{
    this->settings = settings;
    dbName = settings->value("name").toString();
    hostName= settings->value("host").toString();
    user= settings->value("userName").toString();
    password= settings->value("password").toString();
    helperFile = helpersFile;
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
    db.setHostName(hostName);
    // название базы данных
    db.setDatabaseName(dbName);
    // имя пользователя
    db.setUserName(user);
    //пароль
    db.setPassword(password);

    //открытие соединения
    if(db.open()) qDebug("DataBaseConnection : соединение открыто");
    else qDebug("DataBaseConnection : нет соединения. Ошибка %s", qPrintable(db.lastError().text()));
}


QJsonObject DataBaseConnection::getNotifications(QSqlQuery query, QString timestamp)
{
    QJsonObject obj;

    if(timestamp.isEmpty())
        timestamp = getLastTime();

    if(timestamp.contains("error")){
        obj.insert("error", timestamp);
        return obj;
    }

    QString SQL = QString("SELECT process_id, information, error_msg, manager, creation_time "
                          "FROM notification_2 WHERE notification_2.creation_time > '%1' ")
            .arg(timestamp);

    if(!query.prepare(SQL)) {
        qDebug("DataBaseConnection : Prepare fasle. %s", qPrintable(query.lastError().text()));
        obj.insert("error", query.lastError().text());
        return obj;
    }

    if(!query.exec()){
        qDebug("DataBaseConnection : Execution error. %s", qPrintable(query.lastError().text()));
        obj.insert("error", query.lastError().text());
        return obj;
    }

    //забираются данные из ответы
    for(int j = 0; query.next(); j++){
        QJsonObject buff;
        for(int i = 0; i < query.record().count(); i++){
            QString fieldName = query.record().fieldName(i);
            if(fieldName == "creation_time")
                buff.insert(query.record().fieldName(i), query.record().value(i).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            else
                buff.insert(query.record().fieldName(i), query.record().value(i).toJsonValue());
        }
        obj.insert(buff.value("process_id").toString(), buff);
    }

    return obj;
}


QJsonObject DataBaseConnection::insertNotification(QString processId,
                                             QString manager,
                                             QString information,
                                             QString error)
{
    QSqlQuery query = QSqlQuery(db.database(dbName));
    QString SQL;
    QJsonObject obj;

    // no error
    if(error.isEmpty()){
        qDebug() << "no error";
        SQL = QString("INSERT INTO %1 (process_id, information, manager, creation_time) VALUES (?, ?, ?, ?)")
                .arg("notification_2");
    }
    else{
        SQL = QString("INSERT INTO %1 (process_id, information, error_msg, manager, creation_time) VALUES (?, ?, ?, ?, ?)")
                .arg("notification_2");
    }

    if(!query.prepare(SQL)) {
        qDebug() << "with error";
        qDebug("DataBaseConnection : Prepare fasle. %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }

    query.addBindValue(processId);
    query.addBindValue(information);
    if(!error.isEmpty())
        query.addBindValue(error);
    query.addBindValue(manager);
    query.addBindValue(QDateTime::currentDateTime());

    if(!query.exec()){
        qDebug("Запрос: %s", qPrintable(query.lastQuery()));
        qDebug("Запрос не выполнился. Ошибка: %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }

    obj["RestApi"] = "Success";
    return obj;
}

const QString &DataBaseConnection::getDbName() const
{
    return dbName;
}

const QString &DataBaseConnection::getHostName() const
{
    return hostName;
}

const QString &DataBaseConnection::getUser() const
{
    return user;
}

const QString &DataBaseConnection::getPassword() const
{
    return password;
}

QString DataBaseConnection::getLastTime()
{
    QFile file(helperFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        return "error in file opening";
    }
    else{
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        xmlReader.readNext();   // Переходит к первому элементу в файле
        while(!xmlReader.atEnd()){
            if(xmlReader.isStartElement()){
                if(xmlReader.name() == "lastNotif"){
                    return xmlReader.readElementText();
                }
            }
            xmlReader.readNext();
        }
    }

    file.close();
    return "error/ no such token - lastNotif";
}

void DataBaseConnection::setNewLastTime(QString newTime)
{

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
