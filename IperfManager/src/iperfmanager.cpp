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
        connect(qobject_cast<IperfServer*>(iperf),
                &IperfServer::stateChanged,
                this,
                &IperfManager::onProcessStateChaned);
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

void IperfManager::onProcessStateChaned(const QString uuid, ProcessState state)
{
    switch (state) {
    case ProcessState::Starting:
        qDebug("IperfManager : ProcessState::Starting : %s", qPrintable(uuid));
        break;
    case ProcessState::Running:
        qDebug("IperfManager : ProcessState::Running : %s", qPrintable(uuid));
        emit iperfStarted(true);
        emit started();
        break;
    case ProcessState::Finished:
    {
        AbstractIperf *deleted = pool[uuid];

        disconnect(qobject_cast<IperfServer*>(deleted),
                &IperfServer::processStateChaned,
                this,
                &IperfManager::onProcessStateChaned);

        pool.remove(uuid);
        deleted->deleteLater();
        qDebug("IperfManager : onProcessStateChaned : %s удален из пула", qPrintable(uuid));
    }
        break;
    case ProcessState::Crashed:
        qDebug("IperfManager : ProcessState::Crashed : %s", qPrintable(uuid));
        break;
    case ProcessState::FaledToStart:
        qDebug("IperfManager : ProcessState::FaledToStart : %s", qPrintable(uuid));
        emit iperfStarted(false);
        break;
    default:
        break;
    }
}

void IperfManager::onStateChanged(ProcessState state)
{
    AbstractIperf* iperf = qobject_cast<AbstractIperf*>(sender());


}
