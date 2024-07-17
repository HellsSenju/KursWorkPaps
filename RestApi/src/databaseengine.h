#ifndef DATABASEENGINE_H
#define DATABASEENGINE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QSettings>
#include <QSqlDatabase>

class DatabaseEngine : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Класс управления подключениями к бд
    /// \param pool максимальное количество одновременно существующих подключений
    /// \param parent
    ///
    explicit DatabaseEngine(QSettings *settings,
                            int poolSize = 10,
                            QObject *parent = nullptr);


private:
    int poolSize;

    //сегмент настроек для бд
    QString dbName;
    QString hostName;
    QString user;
    QString password;
    //

    QList<QSqlDatabase> dbPool;

    void configureConnections();

signals:

};

#endif // DATABASEENGINE_H
