#ifndef IPERFCLIENT_H
#define IPERFCLIENT_H

#include "abstractiperf.h"

class IperfClient : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfClient(QUuid processUuid);
    ~IperfClient() override;

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

#endif // IPERFCLIENT_H
