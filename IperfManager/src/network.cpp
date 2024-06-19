#include "network.h"

Network::Network(const QSettings* settings, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(settings!=nullptr);

    ip = settings->value("ip").toString();
    port = settings->value("port").toInt();

    manager = new QNetworkAccessManager();
    manager->connectToHost(ip, port);
    connect(manager, &QNetworkAccessManager::finished, this, &Network::onResult);
}

Network::~Network()
{
    manager->deleteLater();
}

Network::Response* Network::post(const QString& url, const QString& ip, int port, QJsonObject body)
{
    QTcpSocket *socket = new  QTcpSocket();

    socket->connectToHost(ip, port);

    if(socket->waitForConnected()){

        QByteArray toSend = configureRequest(url,
                                             ip,
                                             port,
                                             QJsonDocument(body).toJson(QJsonDocument::Compact));

        socket->write(toSend.data(), toSend.length());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QString fromIperf = QString(socket->readAll());

        QString resStatus = fromIperf.split("\r\n").first().split(' ').at(1);
        QString resBody = fromIperf.split("\r\n").last();
        qDebug() << resBody;

        socket->close();
        socket->deleteLater();
        return new Response{resStatus, resBody};
    }
    else
        return nullptr;
}

Network::Response *Network::post(const QString &url, QJsonObject body)
{
    QTcpSocket *socket = new  QTcpSocket();

    socket->connectToHost(ip, port);

    if(socket->waitForConnected()){

        QByteArray toSend = configureRequest(url,
                                             ip,
                                             port,
                                             QJsonDocument(body).toJson(QJsonDocument::Compact));

        socket->write(toSend.data(), toSend.length());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QString fromIperf = QString(socket->readAll());

        QString resStatus = fromIperf.split("\r\n").first().split(' ').at(1);
        QString resBody = fromIperf.split("\r\n").last();
        qDebug() << resBody;

        socket->close();
        socket->deleteLater();
        return new Response{resStatus, resBody};
    }
    else
        return nullptr;
}

QByteArray Network::configureRequest(const QString &url, const QString &ip, int port, const QString &body)
{
    QString str = "";
    str.append(QString("POST %1 HTTP/1.1 \r\n ").arg(url));
    str.append(QString("Host: %1:%2 \r\n").arg(ip).arg(port));
    str.append(QString("Content-Length: %1 \r\n ").arg(body.length()));
    str.append("Content-Type: application/json \r\n ");
    str.append("Connection: keep-alive \r\n ");
    str.append("\r\n");
    if(!body.isEmpty())
        str.append(QString("%1 \r\n").arg(body));
    str.append("\r\n");

    return str.toLocal8Bit();
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



void Network::getData()
{
    QJsonObject toSend{
        {"nfr nfr", "qwertty"}
    };

    QJsonObject obj;
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    qDebug() << data;

    QNetworkRequest request(QUrl("http://localhost:8080/iperf/start"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, toSend.length());

    request.setRawHeader("Connection", "keep-alive");

//    manager->post(request, QJsonDocument(toSend).toJson(QJsonDocument::Compact));
    manager->post(request, data);
}

void Network::onResult(QNetworkReply *reply)
{
    if(reply->error()){
        qDebug() << "error " << reply->errorString();
    }
    else{
        qDebug() << "read all " << QString::fromUtf8(reply->readAll());;
    }

}
