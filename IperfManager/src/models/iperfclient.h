#ifndef IPERFCLIENT_H
#define IPERFCLIENT_H

#include "abstractiperf.h"

class IperfClient : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfClient(QUuid processUuid);
    ~IperfClient();

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
    void processStateChaned(const QString uuid, ProcessState state);

    void stateChanged(ProcessState state);
};

#endif // IPERFCLIENT_H
