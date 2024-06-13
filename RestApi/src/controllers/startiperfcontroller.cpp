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

//            QString str = "";
//            str.append("POST /start HTTP/1.1 \r\n ");
//            str.append(QString("Host: %1:%2 \r\n").arg(8081).arg("localhost"));
//            str.append(QString("Content-Length: %1 \r\n ").
//                       arg(body.length()));
//            str.append("Content-Type: application/json \r\n ");
//            str.append("Connection: keep-alive \r\n ");
//            str.append("\r\n");
//            str.append(body.data());
//            str.append("\r\n");

            QByteArray toSend = configureRequest("localhost", 8081, body);

            socket->write(toSend.data(), toSend.length());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();

            QString fromIperf = QString(socket->readAll());

            QString resStatus = fromIperf.split("\r\n").at(0).split(" ").at(1);
            QString resBody = fromIperf.split("\r\n").at(4);

//            qDebug() << resStatus;
//            qDebug() << resBody;

            if(resStatus == "200"){
                response.setStatus(200,"Ok");
                response.setHeader("Content-Type", "application/json");
            }
            else if(resStatus == "400"){
                response.setStatus(400,"Not Acceptable");
                response.setHeader("Content-Type", "application/json");
            }
            else if(resStatus == "500"){
                response.setStatus(500,"Internal Server Error");
                response.setHeader("Content-Type", "application/json");
            }

            response.write(resBody.toUtf8(), true);
            socket->close();
            socket->deleteLater();
        }
        else{
            qDebug("Not Connected");

            response.setStatus(504, "Gateway Timeout");
            response.setHeader("Content-Type", "application/json");

            QJsonObject object{
                {"rest_api_response", "Не удалось соединиться с IperfManager"}
            };

            response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        }
    }
    else{
        response.setStatus(400, "неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "error 400"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}
