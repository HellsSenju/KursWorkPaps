#include "iperfmanager.h"

IperfManager::IperfManager(QObject *parent)
    : QObject(parent)
{

}

IperfManager::~IperfManager()
{
    for(AbstractIperf* iperf : iperfsPool)
        iperf->deleteLater();

    QMapIterator<QString, AbstractIperf*> i(pool);
    while (i.hasNext()) {
        i.next();
        i.value()->stop();
        i.value()->deleteLater();
    }
}

void IperfManager::startNewProcess(bool server, const QString &uuid, const QString &command)
{
    qDebug("IperfManager : startNewProcess : %s", qPrintable(command));

    AbstractIperf* iperf = nullptr;

    if(server){
        iperf = new IperfServer(uuid);
        connect(qobject_cast<IperfServer*>(iperf), &IperfServer::processStateChaned, this, &IperfManager::onProcessStateChaned);
    }
//    else
//        iperf = new IperfClient(uuid);


    pool.insert(uuid, iperf);

    iperf->setParams("iperf", command.split(' '));

    iperf->start();
}

void IperfManager::stopProcess(const QString &uuid)
{
    qDebug("IperfManager : stopProcess : %s", qPrintable(uuid));

    pool[uuid]->stop();
}

void IperfManager::onProcessStateChaned(const QString &uuid, ProcessState state)
{
    qDebug("IperfManager : onProcessStateChaned : %s", qPrintable(uuid));

    switch (state) {
    case ProcessState::Starting:

        break;
    case ProcessState::Running:

        break;
    case ProcessState::Finished:
        pool[uuid]->deleteLater();
        pool.remove(uuid);
        qDebug("IperfManager : onProcessStateChaned : %s удален из пула", qPrintable(uuid));
        break;
    case ProcessState::Crashed:

        break;
    case ProcessState::FaledToStart:

        break;
    default:
        break;
    }
}
