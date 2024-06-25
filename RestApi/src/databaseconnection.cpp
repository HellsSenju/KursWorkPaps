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


QJsonObject DataBaseConnection::getNotifications(QSqlQuery query, QString from, QString to)
{
    QJsonObject obj;

    if(from.isEmpty())
        from = getLastTime();

    if(from.contains("error")){
        obj.insert("error", from);
        return obj;
    }

    QString SQL;

    if(to.isEmpty())
        SQL = QString("SELECT process_id, information, error_msg, manager, creation_time, command, output_ "
                      "FROM notification_2 WHERE notification_2.creation_time > '%1' ")
        .arg(from);
    else
        SQL = QString("SELECT process_id, information, error_msg, manager, creation_time, command, output_ "
                      "FROM notification_2 WHERE notification_2.creation_time > '%1' "
                      "AND notification_2.creation_time < '%2'")
        .arg(from).arg(to);

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
        obj.insert(QString::number(j), buff);
    }

    if(obj.size() == 0)
        obj.insert("msg", "Новых уведомлений нет.");

    setNewLastTime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    return obj;
}

QJsonObject DataBaseConnection::getStatistic(QSqlQuery query, QString from, QString to)
{
    QJsonObject obj;

    QString SQL = QString("SELECT process_id, interval_, transfer, brandwidth, jitter, lost, total, lost_total, creation_time "
                          "FROM iperf_statistic WHERE iperf_statistic.creation_time > '%1' "
                          "AND iperf_statistic.creation_time < '%2'")
            .arg(from).arg(to);

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
        obj.insert(QString::number(j), buff);
    }

    if(obj.size() == 0)
        obj.insert("msg", QString("Нет записей в промежутке с %1 по %2").arg(from).arg(to));

    return obj;
}


QJsonObject DataBaseConnection::insertNotification(QSqlQuery query,
                                                   QJsonObject data)
{
    QString SQL;
    QJsonObject obj;

    QString from = data.value("from").toString();
    QString error = data.contains("error") ? data.value("error").toString() : "";
    QString msg = "";
    int exitStatus = data.value("exitStatus").toInt();
    int exitCode = data.value("exitCode").toInt();

    if(exitStatus == 0)
        msg.append("Процесс завершился (Normal exit). ");
    else
        msg.append(QString("Процесс завершился с ошибкой = %s (Crashed exit). ").arg(exitCode));

    if(exitCode == 0)
        msg.append("Выполнено без ошибок");
    if(exitCode == 1)
        msg.append("Общая ощибка.");
    if(exitCode == 2)
        msg.append("Неправильное использование команды или аргумента");

    // no error
    if(error.isEmpty() && from == "TCManager"){
        qDebug() << "no error";
        SQL = QString("INSERT INTO %1 (process_id, information, manager, creation_time, command, output_) VALUES (?, ?, ?, ?, ?, ?)")
                .arg("notification_2");
    }
    else if(!error.isEmpty() && from == "TCManager"){
        SQL = QString("INSERT INTO %1 (process_id, information, manager, creation_time, error_msg, command, output_) VALUES (?, ?, ?, ?, ?, ?, ?)")
                .arg("notification_2");
    }
    else if(error.isEmpty() && from == "IperfManager"){
        SQL = QString("INSERT INTO %1 (process_id, information, manager, creation_time, command) VALUES (?, ?, ?, ?, ?)")
                .arg("notification_2");
    }
    else if(!error.isEmpty() && from == "IperfManager"){
        SQL = QString("INSERT INTO %1 (process_id, information, manager, creation_time, error_msg, command) VALUES (?, ?, ?, ?, ?, ?)")
                .arg("notification_2");
    }

    if(!query.prepare(SQL)) {
        qDebug() << "with error";
        qDebug("DataBaseConnection : Prepare fasle. %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }



    query.addBindValue(data.value("uuid").toString());
    query.addBindValue(msg);
    query.addBindValue(from);
    query.addBindValue(QDateTime::currentDateTime());
    if(!error.isEmpty())
        query.addBindValue(error);

    query.addBindValue(data.value("command").toString());

    if(from == "TCManager"){
        query.addBindValue(data.value("output").toString());
    }

    if(!query.exec()){
        qDebug("Запрос: %s", qPrintable(query.lastQuery()));
        qDebug("Запрос не выполнился. Ошибка: %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }

    obj["RestApi"] = "Данные успешно добавлены в уведомления.";
    return obj;
}

QJsonObject DataBaseConnection::insertStatistic(QSqlQuery query, QJsonObject data)
{

    QString SQL = QString("INSERT INTO iperf_statistic(process_id, interval_, transfer, brandwidth, "
                          "jitter, lost, total, lost_total, creation_time) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    QJsonObject obj;

    if(!query.prepare(SQL)) {
        qDebug() << "with error";
        qDebug("DataBaseConnection : Prepare fasle. %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }

    query.addBindValue(data.value("uuid").toString());
    query.addBindValue(data.value("interval").toString());
    query.addBindValue(data.value("transfer").toString());
    query.addBindValue(data.value("bandwidth").toString());
    query.addBindValue(data.value("jitter").toString());
    query.addBindValue(data.value("lost").toInt());
    query.addBindValue(data.value("total").toInt());
    query.addBindValue(data.value("lost/total").toString());
    query.addBindValue(QDateTime::currentDateTime());

    if(!query.exec()){
        qDebug("Запрос: %s", qPrintable(query.lastQuery()));
        qDebug("Запрос не выполнился. Ошибка: %s", qPrintable(query.lastError().text()));
        obj["error"] = query.lastError().text();
        return obj;
    }

    obj["RestApi"] = "Данные успешно добавлены в статистику.";
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
        QString temp = file.readAll();
        file.close();

        QJsonObject json = QJsonDocument::fromJson(temp.toUtf8()).object();

        return json.contains("lastNotif") ? json.value("lastNotif").toString() : "error/ no such field - lastNotif" ;
    }
}

void DataBaseConnection::setNewLastTime(QString newTime)
{
    QFile file(helperFile);
    file.open(QIODevice::WriteOnly | QFile::ReadOnly | QFile::Text);
    QString temp = file.readAll();

    QJsonObject json = QJsonDocument::fromJson(temp.toUtf8()).object();
    json["lastNotif"] = newTime;
    file.resize(0);

    QTextStream stream( &file );
    stream << QJsonDocument(json).toJson(QJsonDocument::Compact);
    file.close();
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
