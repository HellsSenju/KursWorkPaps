#include "network.h"

#include <QUuid>

Network::Network(const QSettings* settings, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(settings != nullptr);

    ip = settings->value("ip").toString();
    port = settings->value("port").toInt();
}

Network::~Network()
{
    QMapIterator<QThread*, HttpSender*> i(pool);
    while (i.hasNext()) {
        i.next();
        deleteThread(i.key());
    }
}

QThread *Network::post(QString url, QString ip, int port, QJsonObject body)
{
    QThread* thread = new QThread();
    HttpSender *sender = new HttpSender(configureRequest(url,
                                                         ip,
                                                         port,
                                                         QJsonDocument(body).toJson(QJsonDocument::Compact)),
                                        ip,
                                        port);

    connect(sender, &HttpSender::finished, thread, &QThread::quit);
    connect(sender, &HttpSender::hadResult, this, &Network::onResult,
            Qt::ConnectionType::QueuedConnection);
    connect(thread, &QThread::started, sender, &HttpSender::run);

    pool.insert(thread, sender);

    sender->moveToThread(thread);
    thread->start();
    return thread;
}

QString Network::post__(QString url, QString ip, int port, QJsonObject body)
{
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QThread* thread = new QThread();
    HttpSender *sender = new HttpSender(configureRequest(url,
                                                         ip,
                                                         port,
                                                         QJsonDocument(body).toJson(QJsonDocument::Compact)),
                                        ip,
                                        port,
                                        uuid);

    connect(thread, &QThread::started, sender, &HttpSender::run);
    connect(thread, &QThread::finished, this, &Network::onThreadFinished);

    connect(sender, &HttpSender::finished, thread, &QThread::terminate);
    connect(sender, &HttpSender::hadResult, this, &Network::onResult,
            Qt::ConnectionType::QueuedConnection);


    pool.insert(thread, sender);

    sender->moveToThread(thread);
    thread->start();
    return uuid;
}

void Network::post(const QString &url, QJsonObject body)
{
    QThread *thread = new QThread();
    HttpSender *sender = new HttpSender(configureRequest(url,
                                                         ip,
                                                         port,
                                                         QJsonDocument(body).toJson(QJsonDocument::Compact)),
                                        ip,
                                        port);

    connect(thread, &QThread::started, sender, &HttpSender::run);
    connect(sender, &HttpSender::finished, thread, &QThread::terminate);
    connect(sender, &HttpSender::hadResult, this, &Network::onResult,
            Qt::ConnectionType::QueuedConnection);
    connect(thread, &QThread::finished, this, &Network::onThreadFinished);

    pool.insert(thread, sender);

    thread->start();
}

void Network::post_(const QString &url, QJsonObject body)
{
    QThread *thread = new QThread();
    HttpSender *sender = new HttpSender(configureRequest(url,
                                                         ip,
                                                         port,
                                                         QJsonDocument(body).toJson(QJsonDocument::Compact)),
                                        ip,
                                        port);

    connect(thread, &QThread::started, sender, &HttpSender::run);
    connect(sender, &HttpSender::finished, thread, &QThread::terminate);
    connect(sender, &HttpSender::hadResult, this, &Network::onResult,
            Qt::ConnectionType::QueuedConnection);
    connect(thread, &QThread::finished, this, &Network::onThreadFinished);

    pool.insert(thread, sender);

    sender->moveToThread(thread);
    thread->start();
}

void Network::deleteThread(QThread *thread)
{
    HttpSender* sender = pool.value(thread);

    disconnect(thread, &QThread::started, sender, &HttpSender::run);
    disconnect(sender, &HttpSender::finished, thread, &QThread::quit);
    disconnect(sender, &HttpSender::hadResult, this, &Network::onResult);

    sender->deleteLater();
    pool.remove(thread);
    thread->deleteLater();
}

void Network::onResult(QJsonObject res, QString uuid)
{
    qDebug() << "onResult : " << res;
    if(!uuid.isEmpty())
        results.insert(uuid, res);
}

void Network::onThreadFinished()
{
    QObject *p = sender();
    QThread* thread = qobject_cast<QThread*>(p);
    HttpSender* sender = pool.value(thread);

    disconnect(thread, &QThread::started, sender, &HttpSender::run);
    disconnect(sender, &HttpSender::finished, thread, &QThread::terminate);
    disconnect(sender, &HttpSender::hadResult, this, &Network::onResult);
    disconnect(thread, &QThread::finished, this, &Network::onThreadFinished);

    sender->deleteLater();
    pool.remove(thread);
    thread->deleteLater();
}

QByteArray Network::configureRequest(const QString &url, const QString &ip, int port, QString body)
{
    QString str = "";
    str.append(QString("POST %1 HTTP/1.1 \r\n ").arg(url));
    str.append(QString("Host: %1:%2 \r\n").arg(ip).arg(port));
    str.append(QString("Content-Length: %1 \r\n ").arg(body.toUtf8().size()));
    str.append("Content-Type: application/json \r\n ");
    str.append("Connection: keep-alive \r\n ");
    str.append("\r\n");
    if(!body.isEmpty())
        str.append(QString("%1 \r\n").arg(body));
    str.append("\r\n");

    return str.toUtf8();
}

QJsonObject Network::parseRequest(const QString &in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object";
        }
    }
    else
    {
        qDebug() << "Invalid JSON" << in;
    }

    return obj;
}



