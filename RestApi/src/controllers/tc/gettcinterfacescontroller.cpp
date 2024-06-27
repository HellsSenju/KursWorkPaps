#include "gettcinterfacescontroller.h"

GetTcInterfacesController::GetTcInterfacesController()
{

}

void GetTcInterfacesController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  network->parseRequest(request.getBody());

    if(!checkRequest(req)){
        response.setStatus(400, "Not Acceptable. неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"RestApi", "Обязательные поля: ip, port, uuid."}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
    else{
        QString ip = req.value("ip").toString();
        int port = req.value("port").toInt();

        QJsonObject body{
            {"uuid", req.value("uuid")}
        };

        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );

        QThread* thread = network->post("/interface", ip, port, body);
        connect( thread, &QThread::finished, &loop, &QEventLoop::quit);

        timer.start(40000); //10 sec
        loop.exec();

        if(timer.isActive()){
            QJsonObject res = network->getResponse(thread);

            QString resStatus = res["resStatus"].toString();
            QJsonObject object =  network->parseRequest(res["resBody"].toString());

            object.insert("host", ip);


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

            response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        }
        else{
            response.setStatus(504, "Gateway Timeout");
            response.setHeader("Content-Type", "application/json");

            QJsonObject object{
                {"RestApi", "Не удалось соединиться с TCManager"}
            };

            response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
        }

        disconnect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
        disconnect( thread, &QThread::finished, &loop, &QEventLoop::quit);

        network->deleteThread(thread);
    }
}

bool GetTcInterfacesController::checkRequest(QJsonObject request)
{
    if(!request.contains("ip") ||
            !request.contains("port") ||
            !request.contains("uuid"))
        return false;

    return true;
}
