#include "httpsender.h"

HttpSender::HttpSender(QByteArray toSend, const QString& ip, int port, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(ip != nullptr);

    this->toSend = toSend;
    this->ip = ip;
    this->port = port;
}

void HttpSender::run()
{
    QTcpSocket *socket = new  QTcpSocket();

    socket->connectToHost(ip, port);

    if(!socket->waitForConnected()){
        qDebug("HttpSender : not connected");
    }

//      qDebug("HttpSender : connected");

    qint64 res = socket->write(toSend);
    if(res == -1){
        qDebug("HttpSender : ничего не было отправлено");
    }
    else{
        qDebug("HttpSender : %lli байт отправлено", res);
    }

    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    QString response = QString(socket->readAll());

    QString resStatus = response.split("\r\n").first().split(' ').at(1);
    QString resBody = response.split("\r\n").last();


    socket->close();
    socket->deleteLater();

    emit finished();
    emit hadResult(resBody);
}
