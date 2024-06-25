#include "httpsender.h"

HttpSender::HttpSender(QByteArray toSend, const QString& ip, int port, QString uuid, QObject *parent)
    : QObject{parent}
{
    Q_ASSERT(ip != nullptr);

    this->toSend = toSend;
    this->ip = ip;
    this->port = port;
    this->uuid = uuid;
}


void HttpSender::run()
{
    QTcpSocket socket;

    socket.connectToHost(ip, port);

    if(!socket.waitForConnected()){
        qDebug("HttpSender : not connected");
        return;
    }

//      qDebug("HttpSender : connected");

    qint64 res = socket.write(toSend);
    if(res == -1){
        qDebug("HttpSender : ничего не было отправлено");
    }
    else{
        qDebug("HttpSender : %lli байт отправлено", res);
    }

    socket.waitForBytesWritten();
    socket.waitForReadyRead();

    QString resStr = QString(socket.readAll());
    if(!resStr.isEmpty()){
        response["resStatus"] = resStr.split("\r\n").first().split(' ').at(1);
        response["resBody"] = resStr.split("\r\n").last();
    }

    socket.close();

    emit hadResult(response, uuid);
    emit finished();
}
