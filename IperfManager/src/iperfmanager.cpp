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

bool IperfManager::getProcessStartStatus(QString uuid)
{
    return pool.value(uuid)->isStarted();
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
                &IperfManager::onStateChanged);
    }
    else{
        iperf = new IperfClient(uuid);
        connect(qobject_cast<IperfClient*>(iperf),
                &IperfClient::stateChanged,
                this,
                &IperfManager::onStateChanged);
    }

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
        AbstractIperf *deleted = pool.value(uuid);

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
    QObject *p = sender();
    AbstractIperf* iperf = qobject_cast<AbstractIperf*>(p);

    switch (state) {
    case ProcessState::Starting:
//        qDebug("IperfManager : ProcessState::Starting : %s", iperf->getUuidChar());
        break;

    case ProcessState::Running:
//        qDebug("IperfManager : ProcessState::Running : %s", iperf->getUuidChar());
        emit iperfStarted(true);
//        emit started();
        break;

    case ProcessState::Finished:
    {
        disconnect(qobject_cast<IperfServer*>(p),
                &IperfServer::stateChanged,
                this,
                &IperfManager::onStateChanged);

        disconnect(qobject_cast<IperfClient*>(p),
                &IperfClient::stateChanged,
                this,
                &IperfManager::onStateChanged);

        pool.remove(iperf->getUuid());
        iperf->deleteLater();
        qDebug("IperfManager : onProcessStateChaned : %s удален из пула", iperf->getUuidChar());
    }
        break;

    case ProcessState::Crashed:
//        qDebug("IperfManager : ProcessState::Crashed : %s", iperf->getUuidChar());
        break;

    case ProcessState::FaledToStart:
//        qDebug("IperfManager : ProcessState::FaledToStart : %s", iperf->getUuidChar());
        emit iperfStarted(false);
        break;

    default:
        break;
    }


}
