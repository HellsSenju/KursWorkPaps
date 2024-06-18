#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &Network::onResult);
}

Network::~Network()
{
    manager->deleteLater();
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
