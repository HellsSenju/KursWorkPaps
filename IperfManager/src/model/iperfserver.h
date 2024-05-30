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
    void output();
    void error();
    bool started();

//    bool finished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif // IPERFSERVER_H
