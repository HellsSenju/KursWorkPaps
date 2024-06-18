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


class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    ~Network();

public slots:
    void getData();
    void onResult(QNetworkReply *reply);

signals:
    void onReady();

private:
    QNetworkAccessManager* manager;
};

#endif // NETWORK_H
