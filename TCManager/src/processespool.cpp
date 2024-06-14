#include "processespool.h"

ProcessesPool::ProcessesPool(QObject *parent)
    : QObject{parent}
{

}

ProcessesPool::~ProcessesPool()
{
    QMapIterator<QString, AbstractProcess*> i(pool);
    while (i.hasNext()) {
        i.next();
        i.value()->stop();
        i.value()->deleteLater();
    }
}

void ProcessesPool::execute(const QString &uuid, Programs program, const QString &command)
{
    AbstractProcess* process = nullptr;
    if(program == Programs::TC){
        process = new TCProcess(uuid);
        process->setParams(tc, command.split(' '));
    }
    else if(program == Programs::NMCLI){
        process = new Process(uuid);
        process->setParams(nmcli, command.split(' '));
    }

    connect(process, &AbstractProcess::stateChanged,
            this, &ProcessesPool::onStateChanged);

    pool.insert(uuid, process);

    process->execute();
}

void ProcessesPool::deleteProcess(const QString &uuid)
{
    pool.value(uuid)->deleteLater();
    pool.remove(uuid);
}

void ProcessesPool::onStateChanged(ProcessState state)
{
    QObject *p = sender();
    AbstractProcess* process = qobject_cast<AbstractProcess*>(p);


    switch (state) {
    case ProcessState::Finished:
        disconnect(process, &AbstractProcess::stateChanged,
                   this, &ProcessesPool::onStateChanged);

        qDebug("ProcessesPool : onProcessStateChaned : %s удален из пула", process->getUuidChar());
        break;

    case ProcessState::Crashed:
        qDebug("ProcessesPool : onProcessStateChaned : %s crashed", process->getUuidChar());
        break;

    case ProcessState::FailedToStart:
        qDebug("ProcessesPool : onProcessStateChaned : %s failed to start", process->getUuidChar());
        break;

    default:
        break;
    }

    emit executed();
}
