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

    response.setStatus(200,"Ok");
    response.setHeader("Content-Type", "application/json");

    QJsonObject object{
        {"port", 2314 },
        {"protocol", "-u"}
    };

    response.write(QJsonDocument(object).toJson(QJsonDocument::Compact), true);

}
