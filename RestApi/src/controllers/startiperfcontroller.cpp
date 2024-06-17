#include "startiperfcontroller.h"

StartIperfController::StartIperfController()
{

}

void StartIperfController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(checkRequest(req)){
        response.setStatus(400, "Not Acceptable. неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"RestApi", "Обязательные поля: ip, port, uuid, mode, server_ip (if mode=-c)."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        return;
    }

    QString ip = req.value("ip").toString();
    int port = req.value("port").toInt();

    QJsonObject body{
        {"uuid", req.value("uuid")},
        {"command", configureParams(req)},
        {"mode", req.value("mode")}
    };

    QTcpSocket *socket = new  QTcpSocket(this);

    socket->connectToHost(ip, port);

    if(socket->waitForConnected()){

        QByteArray toSend = configureRequest("/start",
                                             ip,
                                             port,
                                             QJsonDocument(body).toJson(QJsonDocument::Compact));

        socket->write(toSend.data(), toSend.length());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        QString fromIperf = QString(socket->readAll());

        QString resStatus = fromIperf.split("\r\n").first().split(' ').at(1);
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

bool StartIperfController::checkRequest(QJsonObject request)
{
    if(!request.contains("ip") ||
            !request.contains("port") ||
            !request.contains("uuid") ||
            !request.contains("mode"))
        return false;

    if(request.value("mode").toString() != "-c" || request.value("mode").toString() != "-s" )
        return false;

    if(request.value("mode").toString() == "-c" && !request.contains("server_ip"))
        return false;

    return true;
}

QString StartIperfController::configureParams(QJsonObject request)
{
    QString params;
    params.append(request.value("mode").toString());
    params.append(" ");

    if(request.contains("server_ip")){
        params.append(request.value("server_ip").toString());
        params.append(" ");
    }

    if(request.contains("server_port")){
        params.append(QString("-p %1 ").arg(request.value("server_port").toInt()));
    }

    if(request.contains("protocol")){
        params.append(request.value("protocol").toString());
        params.append(" ");
    }

    if(request.contains("interval")){
        params.append(QString("-i %1 ").arg(request.value("interval").toInt()));
    }

    if(request.contains("time")){
        params.append(QString("-t %1 ").arg(request.value("time").toInt()));
    }

    qDebug() << "params: " << params;
    return params;
}
