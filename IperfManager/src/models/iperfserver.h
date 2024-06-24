#ifndef IPERFSERVER_H
#define IPERFSERVER_H

#include "abstractiperf.h"


class IperfServer : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfServer(QUuid processUuid);

protected slots:
    void onStandartOutput() override;

};

#endif // IPERFSERVER_H
