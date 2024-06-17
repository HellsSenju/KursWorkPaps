#include "stopiperfcontroller.h"

StopIperfController::StopIperfController()
{

}

void StopIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(checkRequest(req)){
        response.setStatus(400, "Not Acceptable. неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"RestApi", "Обязательные поля: ip, port, uuid."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        return;
    }

    QString uuid = req.value("uuid").toString();
    QString ip = req.value("ip").toString();
    int port = req.value("port").toInt();

    req.remove("ip");
    req.remove("port");

    QTcpSocket *socket = new  QTcpSocket(this);

    socket->connectToHost(ip, port);

    if(socket->waitForConnected()){
        QByteArray toSend = configureRequest("/stop",
                                             ip,
                                             port,
                                             QJsonDocument(req).toJson(QJsonDocument::Compact));

        socket->write(toSend.data(), toSend.length());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QString fromIperf = QString(socket->readAll());

        QString resStatus = fromIperf.split("\r\n").first().split(" ").at(1);
        QString resBody = fromIperf.split("\r\n").last();

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
            {"RestApi", "Не удалось соединиться с IperfManager"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}

bool StopIperfController::checkRequest(QJsonObject request)
{
    if(!request.contains("ip") || !request.contains("port") || !request.contains("uuid"))
        return false;

    return true;
}
