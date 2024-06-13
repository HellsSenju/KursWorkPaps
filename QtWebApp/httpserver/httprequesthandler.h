/**
  @file
  @author Stefan Frings
*/

#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include "httpglobal.h"
#include "httprequest.h"
#include "httpresponse.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QStringList>
#include <QEventLoop>
#include <QObject>

namespace stefanfrings {

/**
   The request handler generates a response for each HTTP request. Web Applications
   usually have one central request handler that maps incoming requests to several
   controllers (servlets) based on the requested path.
   <p>
   You need to override the service() method or you will always get an HTTP error 501.
   <p>
   @warning Be aware that the main request handler instance must be created on the heap and
   that it is used by multiple threads simultaneously.
   @see StaticFileController which delivers static local files.
*/

class DECLSPEC HttpRequestHandler : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(HttpRequestHandler)
public:

    /**
     * Constructor.
     * @param parent Parent object.
     */
    HttpRequestHandler(QObject* parent=nullptr);

    /** Destructor */
    virtual ~HttpRequestHandler();

    /**
      Generate a response for an incoming HTTP request.
      @param request The received HTTP request
      @param response Must be used to return the response
      @warning This method must be thread safe
    */
    virtual void service(HttpRequest& request, HttpResponse& response);

    virtual QJsonObject parseRequest(const QString& in)
    {
        QJsonObject obj;

        QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

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
    };

    virtual QByteArray configureRequest(const QString& ip,
                                        int port,
                                        const QString& body)
    {
        QString str = "";
        str.append("POST /start HTTP/1.1 \r\n ");
        str.append(QString("Host: %1:%2 \r\n").arg(ip).arg(port));
        str.append(QString("Content-Length: %1 \r\n ").arg(body.length()));
        str.append("Content-Type: application/json \r\n ");
        str.append("Connection: keep-alive \r\n ");
        str.append("\r\n");
        str.append(QString("%1").arg(body));
        str.append("\r\n");

        return str.toLocal8Bit();
    };
};

} // end of namespace

#endif // HTTPREQUESTHANDLER_H
