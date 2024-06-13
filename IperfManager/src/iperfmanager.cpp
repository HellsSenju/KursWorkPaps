#include "iperfmanager.h"

IperfManager::IperfManager(QObject *parent)
    : QObject(parent)
{

}

IperfManager::~IperfManager()
{
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
    }
    else{
        iperf = new IperfClient(uuid);
    }

    connect(iperf,
            &AbstractIperf::stateChanged,
            this,
            &IperfManager::onStateChanged);

    pool.insert(uuid, iperf);

    iperf->setParams("iperf", command.split(' '));
    iperf->start();
}

void IperfManager::stopProcess(const QString &uuid)
{
    qDebug("IperfManager : stopProcess : %s", qPrintable(uuid));

    pool[uuid]->stop();
}


void IperfManager::onStateChanged(ProcessState state)
{
    QObject *p = sender();
    AbstractIperf* iperf = qobject_cast<AbstractIperf*>(p);

    switch (state) {
    case ProcessState::Starting:
        break;

    case ProcessState::Running:
        emit iperfStarted();
        break;

    case ProcessState::Finished:
    {
        disconnect(iperf, &AbstractIperf::stateChanged,
                    this, &IperfManager::onStateChanged);

        pool.remove(iperf->getUuid());
        iperf->deleteLater();
        qDebug("IperfManager : onProcessStateChaned : %s удален из пула", iperf->getUuidChar());
    }
        break;

    case ProcessState::Crashed:
        break;

    case ProcessState::FailedToStart:
        emit iperfStarted();
        break;

    default:
        break;
    }


}
