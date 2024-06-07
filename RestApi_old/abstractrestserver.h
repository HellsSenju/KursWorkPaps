#ifndef ABSTRACTRESTSERVER_H
#define ABSTRACTRESTSERVER_H

#ifndef Q_MOC_RUN
#define NO_AUTH_REQUIRED
#endif

#include <QTcpServer>
#include <QTcpSocket>
#include <QMutex>
#include <QList>
#include <QSet>
#include <QString>
#include <QHash>

#include "workerthread.h"

class AbstractRestServer : QTcpServer
{
public:
    explicit AbstractRestServer(QString &pathPrefix, int port, QObject *parent = 0);
Q_INVOKABLE void startListen();
Q_INVOKABLE void stopListen();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:

    QThread *m_serverThread = nullptr;
    QList<WorkerThreadInfo> m_threadPool;
    QSet<QTcpSocket *> m_socket;
    QMutex m_socketMutex;
    MethodNode m_methodTreeRoot;
    int m_maxThreadsCount;


    void tryToCallMethod(QTcpSocket *socket, const QString &type, const QString &method,
                         QStringList headers, const QByteArray &body);

    QStringList makeMethodName(const QString &type, const QString &name);

    MethodNode *findMethod(const QStringList &splitterMethod, QString &methodVariavlePath);

    void fillMethod();

    void addMethodToTree(const QString &realMethod, const QString &tag);

    void sendAnswer(QTcpSocket *socket, QByteArray &body, const QString &contentType,
                    const QHash<QString, QString> &headers,
                    int returnCode = 200, const QString &reason = QString());

    void registerSocket(QTcpSocket *socket);
    void deleteSocket(QTcpSocket *socket, WorkerThread *worker);



};

#endif // ABSTRACTRESTSERVER_H
