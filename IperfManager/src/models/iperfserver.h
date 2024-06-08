#ifndef IPERFSERVER_H
#define IPERFSERVER_H

#include "abstractiperf.h"


class IperfServer : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfServer(QUuid processUuid);
    ~IperfServer();

    void setParams(const QString &program, const QStringList &args);
    void start();
    void stop();
    void waitForFinished(int sec = 30);

private slots:
    void onStandartOutput();
    void onErrorOccurred(QProcess::ProcessError error);
    void onStandartError();
    void onStarted();

signals:
//    void started(const QString &uuid);
//    void finished(const QString &uuid)e;

    void processStateChaned(const QString uuid, ProcessState state);

    void stateChanged(ProcessState state);

};

#endif // IPERFSERVER_H
