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
                        QString uuid = "",
                        QObject *parent = nullptr);

    /** получение ответа от сокета */
    const QJsonObject &getResponse() const{
        return response;
    };

private:
    QByteArray toSend;
    QString uuid;
    QString ip;
    int port;

    QJsonObject response;

public slots:
    /** логика выполения подключения и получения ответа */
    void run();

signals:
    void finished();
    void hadResult(QJsonObject res);

};

#endif // HTTPSENDER_H
