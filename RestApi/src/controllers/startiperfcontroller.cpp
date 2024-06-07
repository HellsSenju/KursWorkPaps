#include "startiperfcontroller.h"

StartIperfController::StartIperfController()
{

}

void StartIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QByteArray body = request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server") || req.contains("client")){

        QTcpSocket *socket = new  QTcpSocket(this);

        socket->connectToHost(QHostAddress::LocalHost, 8081);

        if(socket->waitForConnected()){
            qDebug("Connected!");

            QString str = "";
            str.append("POST /start HTTP/1.1 \r\n ");
            str.append("Host: localhost:8081 \r\n ");
            str.append(QString("Content-Length: %1 \r\n ").
                       arg(body.length()));
            str.append("Content-Type: application/json \r\n ");
            str.append("Connection: keep-alive \r\n ");
            str.append("\r\n");
            str.append(body.data());
            str.append("\r\n");
            QByteArray toSend = str.toLocal8Bit();
            int size = str.toLocal8Bit().length();

            socket->write(toSend.data(), size);
            socket->waitForBytesWritten();
            socket->waitForReadyRead();


//            socket->waitForDisconnected();

        }
        else{
            qDebug("Not Connected");

            response.setStatus(504, "Gateway Timeout");
            response.setHeader("Content-Type", "application/json");

            QJsonObject object{
                {"rest_api_response", "Не удалось связаться с IperfManager"}
            };

            response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        }

        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "Запрос принят и передан дальше"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
    else{
        response.setStatus(400, "неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "ляяяяяяяяяяя работает - 400"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}
