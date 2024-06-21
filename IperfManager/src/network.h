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

    struct Response{
        QString resStatus;
        QString resBody;
    };

    Response* post(const QString &url, const QString &ip, int port, QJsonObject body);

    void post(const QString &url, QJsonObject body);

    QByteArray configureRequest(const QString& url,
                                const QString& ip,
                                int port,
                                QString body);

    QByteArray configureHeaders(const QString& url,
                                const QString& ip,
                                int port);

    QJsonObject parseRequest(const QString& in);


public slots:
    void onResult(QString res);
    void onThreadFinished();

signals:
    void onReady();


private:
    QString ip;
    int port;

    QMap<QThread*, HttpSender*> pool;

};

#endif // NETWORK_H
