#ifndef IPERFCLIENT_H
#define IPERFCLIENT_H

#include "abstractiperf.h"

class IperfClient : public AbstractIperf
{
    Q_OBJECT

public:
    explicit IperfClient(QUuid processUuid);

};

#endif // IPERFCLIENT_H
