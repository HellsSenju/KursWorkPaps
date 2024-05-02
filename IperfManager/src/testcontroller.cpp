#include "testcontroller.h"
#include "glob.h"
#include <QJsonObject>
#include <QJsonDocument>

TestController::TestController()
{

}

void TestController::service(HttpRequest &request, HttpResponse &response)
{
    qDebug() << request.getBody();
    QJsonObject req =  parseRequest(request.getBody());

    if(req.contains("server")){
        response.setStatus(200,"Ok");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"response", "ляяяяяяяяяяя работает"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }

    else{
        response.setStatus(400, "неправильный, некорректный запрос");
        response.setHeader("Content-Type", "application/json");

        QJsonObject object{
            {"response", "ляяяяяяяяяяя работает - 400"}
        };

        response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);
    }
}

QJsonObject TestController::parseRequest(const QString& in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
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
