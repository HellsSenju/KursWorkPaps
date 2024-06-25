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

    struct Response{
        QString resStatus;
        QString resBody;
    };

    QThread *post(QString url, QString ip, int port, QJsonObject body);

    void post(const QString &url, QJsonObject body);

    void deleteThread(QThread *thread);

    QJsonObject getResponse(QThread* thread){
        return pool.value(thread)->getResponse();
    };

    QByteArray configureRequest(const QString& url,
                                const QString& ip,
                                int port,
                                QString body);

    QByteArray configureHeaders(const QString& url,
                                const QString& ip,
                                int port);

    QJsonObject parseRequest(const QString& in);

public slots:
    void onResult(QJsonObject res);
    void onThreadFinished();

signals:
    void onReady();


private:
    QString ip;
    int port;

    QMap<QThread*, HttpSender*> pool;


};

#endif // NETWORK_H
