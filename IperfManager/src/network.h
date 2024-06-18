#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include <QSettings>


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

    Response* post(const QString &url, const QString &ip, int port, QJsonObject body);

    Response *post(const QString &url, QJsonObject body);

    QByteArray configureRequest(const QString& url,
                                const QString& ip,
                                int port,
                                const QString& body);

    QJsonObject parseRequest(const QString& in);


public slots:
    void getData();
    void onResult(QNetworkReply *reply);

signals:
    void onReady();

private:
    QNetworkAccessManager* manager;
    QString ip;
    int port;


};

#endif // NETWORK_H
