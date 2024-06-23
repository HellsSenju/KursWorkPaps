#ifndef HTTPSENDER_H
#define HTTPSENDER_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>


class HttpSender : public QObject
{
    Q_OBJECT
public:
    explicit HttpSender(QByteArray toSend,
                        const QString &ip,
                        int port,
                        QObject *parent = nullptr);

    const QJsonObject &getResponse() const{
        return response;
    };

    const QString &getUuid() const{
        return uuid;
    };

private:
    QByteArray toSend;
    QString uuid;
    QString ip;
    int port;

    QJsonObject response;



public slots:
    void run();

signals:
    void finished();
    void written(bool res);
    void hadResult(QString res);

};

#endif // HTTPSENDER_H
