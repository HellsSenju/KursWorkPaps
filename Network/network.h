#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include <QSettings>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QThread>

#include "httpsender.h"


class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(const QSettings* settings, QObject *parent = nullptr);
    ~Network();

    /** запрос (используется апи для запросов к менеджерам) */
    QThread *post(QString url, QString ip, int port, QJsonObject body);

    /** запрос (используется менеджерами для отправки в апи информации) */
    void post(const QString &url, QJsonObject body);

    /** удаление потока без использования сигналов-слотов */
    void deleteThread(QThread *thread);

    /** получение ответа */
    QJsonObject getResponse(QThread* thread){
        return pool.value(thread)->getResponse();
    };

    /** конфигурация запрсов для отправки по сокету */
    QByteArray configureRequest(const QString& url,
                                const QString& ip,
                                int port,
                                QString body);

    /** конфигурация запрсов для отправки по сокету */
    QByteArray configureHeaders(const QString& url,
                                const QString& ip,
                                int port);

    /** парсинг реквеста */
    QJsonObject parseRequest(const QString& in);

public slots:
    void onResult(QJsonObject res);
    /** остановка процесса с использованием сигналов-слотов */
    void onThreadFinished();

signals:
    void onReady();


private:
    QString ip;
    int port;

    QMap<QThread*, HttpSender*> pool;


};

#endif // NETWORK_H
