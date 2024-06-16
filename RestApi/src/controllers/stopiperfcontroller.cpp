#include "stopiperfcontroller.h"

StopIperfController::StopIperfController()
{

}

void StopIperfController::service(HttpRequest &request, HttpResponse &response)
{
    QString uuid = request.getParameter("uuid");

    if(uuid.isEmpty()){
        response.setStatus(400, "неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "error 400"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        return;
    }


    QTcpSocket *socket = new  QTcpSocket(this);

    socket->connectToHost(QHostAddress::LocalHost, 8081);

    if(socket->waitForConnected()){
        QByteArray toSend = configureRequest(QString("/stop?uuid=%1").arg(uuid), "localhost", 8081, "");

        socket->write(toSend.data(), toSend.length());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QString fromIperf = QString(socket->readAll());

        QString resStatus = fromIperf.split("\r\n").at(0).split(" ").at(1);
        QString resBody = fromIperf.split("\r\n").at(4);

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
        response.setStatus(504, "Gateway Timeout");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"rest_api_response", "Не удалось соединиться с IperfManager"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}
