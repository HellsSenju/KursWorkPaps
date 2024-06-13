#include "processespool.h"

ProcessesPool::ProcessesPool(QObject *parent)
    : QObject{parent}
{

}

ProcessesPool::~ProcessesPool()
{
    QMapIterator<QString, TCProcess*> i(pool);
    while (i.hasNext()) {
        i.next();
        i.value()->stop();
        i.value()->deleteLater();
    }
}

void ProcessesPool::execute(const QString &uuid, const QString &command)
{
    TCProcess* process = new TCProcess(uuid);
    connect(process, &TCProcess::stateChanged,
            this, &ProcessesPool::onStateChanged);

    pool.insert(uuid, process);

    process->setParams("sudo /usr/sbin/tc", command.split(' '));
    process->execute();
}

void ProcessesPool::onStateChanged(ProcessState state)
{
    QObject *p = sender();
    TCProcess* tc = qobject_cast<TCProcess*>(p);


    switch (state) {
    case ProcessState::Finished:
        disconnect(tc, &TCProcess::stateChanged,
                   this, &ProcessesPool::onStateChanged);

//        pool.remove(tc->getUuid());
//        tc->deleteLater();

        qDebug("ProcessesPool : onProcessStateChaned : %s удален из пула", tc->getUuidChar());
        break;

    case ProcessState::Crashed:
        qDebug("ProcessesPool : onProcessStateChaned : %s crashed", tc->getUuidChar());
        break;

    case ProcessState::FailedToStart:
        qDebug("ProcessesPool : onProcessStateChaned : %s failed to start", tc->getUuidChar());
        break;

    default:
        break;
    }

    emit executed();
}
