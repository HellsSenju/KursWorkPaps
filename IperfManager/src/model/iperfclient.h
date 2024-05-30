#ifndef IPERFCLIENT_H
#define IPERFCLIENT_H

#include "abstractiperf.h"

class IperfClient : public AbstractIperf
{
    Q_OBJECT

public:
    IperfClient(QUuid processUuid);
    ~IperfClient();

    void setParams(const QString &program, const QStringList &args);
    void start();
    void stop();
    void waitForFinished(int sec = 30);

private slots:
    void output();
    void error();
    bool started();

};

#endif // IPERFCLIENT_H
