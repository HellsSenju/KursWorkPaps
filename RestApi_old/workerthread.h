#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

#include "abstractrestserver.h"

class WorkerThread: public QThread
{
public:
    WorkerThread();
};

#endif // WORKERTHREAD_H
