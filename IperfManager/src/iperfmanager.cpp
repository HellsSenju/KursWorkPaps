#include "iperfmanager.h"

IperfManager::IperfManager(QObject *parent)
    : QObject(parent)
{

}

IperfManager::~IperfManager()
{
    for(AbstractIperf* iperf : iperfsPool)
        iperf->deleteLater();
}

void IperfManager::onStart(bool server, const QString &uuid, const QString &command)
{
    qDebug() << command;

    AbstractIperf* iperf;

    if(server)
        iperf = new IperfServer(uuid);
    else
        iperf = new IperfClient(uuid);

    iperfsPool.append(iperf);

    iperf->setParams("iperf", QStringList{ command });

    iperf->start();
}
