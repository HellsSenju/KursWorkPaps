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

bool IperfManager::startNewProcess(bool server, const QString &uuid, const QString &command)
{
    qDebug("IperfManager : Попытка запустить процесс : %s", qPrintable(command));

    if(pool.contains(uuid))
        return false;

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
    return true;
}

bool IperfManager::stopProcess(const QString &uuid)
{
    qDebug("IperfManager : остановка процесса : %s", qPrintable(uuid));
    if(!pool.contains(uuid))
        return false;

    pool.value(uuid)->stop();
    return true;
}

void IperfManager::deleteProcess(const QString &uuid)
{
    pool.value(uuid)->deleteLater();
    pool.remove(uuid);
    qDebug("IperfManager : Процесс удален из пула : %s", qPrintable(uuid));
}

void IperfManager::onStateChanged(ProcessState state)
{
    QObject *p = sender();
    AbstractIperf* iperf = qobject_cast<AbstractIperf*>(p);

    switch (state) {
    case ProcessState::Starting:
        break;

    case ProcessState::Running:
        emit iperfChanged();
        break;

    case ProcessState::Finished:
    {
        disconnect(iperf, &AbstractIperf::stateChanged,
                    this, &IperfManager::onStateChanged);

        emit iperfChanged();
    }
        break;

    case ProcessState::Crashed:
        break;

    case ProcessState::FailedToStart:
        emit iperfChanged();
        break;
    }
}
