#ifndef API_H
#define API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Api : public QObject
{
    Q_OBJECT
public:
    explicit Api(QObject *parent = nullptr);


private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

signals:

};

#endif // API_H
