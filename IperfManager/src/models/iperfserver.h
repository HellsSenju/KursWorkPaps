#ifndef IPERFSERVER_H
#define IPERFSERVER_H

#include "abstractiperf.h"


class IperfServer : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfServer(QUuid processUuid);
    ~IperfServer() override;

    void setParams(const QString &program, const QStringList &args) override;
    void start() override;
    void stop() override;
    void waitForFinished(int sec = 30) override;

private slots:
    void onStandartOutput();
    void onErrorOccurred(QProcess::ProcessError error);
    void onStandartError();
    void onStarted();

};

#endif // IPERFSERVER_H
