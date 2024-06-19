#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include <QSettings>
#include <QTcpSocket>


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

    Response *post(const QString &url, QJsonObject body);

    QByteArray configureRequest(const QString& url,
                                const QString& ip,
                                int port,
                                const QString& body);

    QJsonObject parseRequest(const QString& in);


public slots:


signals:
    void onReady();

private:
    QString ip;
    int port;


};

#endif // NETWORK_H
